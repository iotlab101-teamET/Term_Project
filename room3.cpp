#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <ConfigPortal8266.h>
#include <Servo.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <SSD1306.h>
SSD1306   display(0x3c, 4, 5, GEOMETRY_128_32);

//#define PIR 16
//#define LED 15
#define PIN            15  // Neopixel 데이터 핀
#define NUMPIXELS      8  // Neopixel 개수

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

char*               ssid_pfix = (char*)"yong";
String              user_config_html = ""
    "<p><input type='text' name='mqttServer' placeholder='mqtt Broker'>";

DHTesp              dht;
unsigned long       interval = 2000;
unsigned long       lastDHTReadMillis = 0;
float               humidity = 0;
float               temperature = 0;
 
const char*         mqttServer;
const int           mqttPort = 1883;

unsigned long       pubInterval = 2000;
unsigned long       lastPublished = - pubInterval;

WiFiClient espClient;
PubSubClient client(espClient);
void readDHT22();
void callback(char* topic, byte* payload, unsigned int length);

Servo myservo;
int pos = 0;

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA); 
    loadConfig();
    pixels.begin();
    
    if(!cfg.containsKey("config") || strcmp((const char*)cfg["config"], "done")) {
        configDevice();
    }
    WiFi.begin((const char*)cfg["ssid"], (const char*)cfg["w_pw"]);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    mqttServer = cfg["mqttServer"];
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    while (!client.connected()) {
        Serial.println("Connecting to MQTT...");
 
        if (client.connect("room3")) {
            Serial.println("connected");  
        } else {
            Serial.print("failed with state "); Serial.println(client.state());
            delay(2000);
        }
    }
    dht.setup(14, DHTesp::DHT22);   // DHT22센서를 GPIO 14번 핀에 연결
    myservo.attach(16);  // 16번 핀으로 PWM 사용
    client.subscribe("TeamET/room3/neopixel/cmd");
    client.subscribe("TeamET/room3/servo/cmd");
}

void loop() {
    client.loop();
    
    unsigned long currentMillis = millis();
    if(currentMillis - lastPublished >= pubInterval) {
        lastPublished = currentMillis;
        readDHT22();
        int light = analogRead(0);
        Serial.printf("%.1f\t %.1f\t %d\n", temperature, humidity, light);
        char buf[10];
        sprintf(buf, "%.1f", temperature);
        client.publish("TeamET/room3/sensor/evt/temp", buf);
        sprintf(buf, "%.1f", humidity);
        client.publish("TeamET/room3/sensor/evt/humid", buf);
        sprintf(buf, "%d", light);
        client.publish("TeamET/room3/sensor/evt/light", buf);
    }
}

void readDHT22() {
    unsigned long currentMillis = millis();

    if(currentMillis - lastDHTReadMillis >= interval) {
        lastDHTReadMillis = currentMillis;

        humidity = dht.getHumidity();              // 습도 값 읽어오기
        temperature = dht.getTemperature();        // 온도 값 읽어오기
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
 
    char msgBuffer[20];

    if(!strcmp(topic, "TeamET/room3/neopixel/cmd")) {
        int i;
        for(i = 0; i < (int)length; i++) {
            msgBuffer[i] = payload[i];
        } 
        msgBuffer[i] = '\0';
        Serial.printf("\n%s -> %s", topic, msgBuffer);
        if(!strcmp(msgBuffer, "blue")) {
            pixels.setPixelColor(2, pixels.Color(0, 0, 255));
            pixels.show();
        }
        if(!strcmp(msgBuffer, "red")) {
            pixels.setPixelColor(0, pixels.Color(255, 0, 0));
            pixels.show();
        }
        if(!strcmp(msgBuffer, "green")) {
            pixels.setPixelColor(1, pixels.Color(0, 255, 0));
            pixels.show();
        }
        if(!strcmp(msgBuffer, "white")) {
            pixels.setPixelColor(3, pixels.Color(255, 255, 255));
            pixels.show();
        }

        if(!strcmp(msgBuffer, "bluex")) {
            pixels.setPixelColor(2, pixels.Color(0, 0, 0));
            pixels.show();  // 여기서 변경된 부분
        }
        if(!strcmp(msgBuffer, "redx")) {
            pixels.setPixelColor(0, pixels.Color(0, 0, 0));
            pixels.show();  // 여기서 변경된 부분c
        }
        if(!strcmp(msgBuffer, "greenx")) {
            pixels.setPixelColor(1, pixels.Color(0, 0, 0));
            pixels.show();  // 여기서 변경된 부분
        }
        if(!strcmp(msgBuffer, "whitex")) {
            pixels.setPixelColor(3, pixels.Color(0, 0, 0));
            pixels.show();  // 여기서 변경된 부분
        }
    }

    else if(!strcmp(topic, "TeamET/room3/servo/cmd")) {
        int i;
        for(i = 0; i < (int)length; i++) {
            msgBuffer[i] = payload[i];
        } 
        msgBuffer[i] = '\0';
        Serial.printf("\n%s -> %s", topic, msgBuffer);

        if(!strcmp(msgBuffer, "open")) {
            pos = 180;
            myservo.write(pos);
        }
        if(!strcmp(msgBuffer, "close")) 
        {
          pos = 0;
          myservo.write(pos);
        }
    }
}
