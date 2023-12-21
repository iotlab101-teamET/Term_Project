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

#define NOTE_C4  262    
#define NOTE_D4  294    //이렇게 Define하는 이유는 int나 float long double등으로 변수를 생성 할시
#define NOTE_E4  330    //메모리에 데이터가 저장되므로 데이터가 쓰인다. 하지만 Define은 뒤에 ';'가 붙지 않는다.
#define NOTE_F4  349    //상수는 메모리를 차지하지 않는다.
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988
#define NOTE_C6 1047 // 도 (6옥타브)
#define NOTE_CS6 1109
#define NOTE_D6 1175 // 레 (6옥타브)
#define NOTE_DS6 1245
#define NOTE_E6 1319 // 미 (6옥타브)
#define NOTE_F6 1397 // 파 (6옥타브)
#define NOTE_FS6 1480
#define NOTE_G6 1568 // 솔 (6옥타브)
#define NOTE_GS6 1661
#define NOTE_A6 1760 // 라 (6옥타브)
#define NOTE_AS6 1865
#define NOTE_B6 1976 // 시 (6옥타브)
#define NOTE_C7 2093 // 도 (7옥타브)

// 징글벨 첫 소절 음계 : 알기 쉽게 다장조의 음계로 변환하여 처리
int song[] = {500, 525, 550, 575, 600, 625, 650, 675, 700, 725,
        750, 775, 800, 825, 850, 875, 900, 925, 950, 975,
        1000, 1025, 1050, 1075, 1100, 1125, 1150, 1175, 1200, 1225,
        1250, 1275, 1300, 1325, 1350, 1375, 1400, 1425, 1450, 1475,
        1500, 1475, 1450, 1425, 1400, 1375, 1350, 1325, 1300, 1275,
        1250, 1225, 1200, 1175, 1150, 1125, 1100, 1075, 1050, 1025,
        1000, 975, 950, 925, 900, 875, 850, 825, 800, 775,
        750, 725, 700, 675, 650, 625, 600, 575, 550, 525,
        500}; // 솔솔파레미도(쉼표)

// 징글벨 첫 소절 연주시간
int TimeTime = 50;

//#define PIR 16
//#define LED 15
#define PIN            15  // Neopixel 데이터 핀
#define NUMPIXELS      8  // Neopixel 개수
#define PIEZO 16
#define flame 13

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

char*               ssid_pfix = (char*)"SJ_A";
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

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA); 
    loadConfig();
    pixels.begin();
    pinMode(flame, INPUT);
    pinMode(PIEZO, OUTPUT); // PIEZO 버저핀은 출력
    
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
 
        if (client.connect("Lroom-A")) {
            Serial.println("connected");  
        } else {
            Serial.print("failed with state "); Serial.println(client.state());
            delay(2000);
        }
    }
    dht.setup(14, DHTesp::DHT22);   // DHT22센서를 GPIO 14번 핀에 연결
    client.subscribe("TeamET/Lroom/neopixel/cmd");
    client.subscribe("TeamET/Lroom/buzzer/cmd");
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
        client.publish("TeamET/Lroom/sensor/evt/temp", buf);
        sprintf(buf, "%.1f", humidity);
        client.publish("TeamET/Lroom/sensor/evt/humid", buf);
        sprintf(buf, "%d", light);
        client.publish("TeamET/Lroom/sensor/evt/light", buf);
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

    if(!strcmp(topic, "TeamET/Lroom/neopixel/cmd")) {
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
        //Serial.printf("\n%s -> %s", topic, msgBuffer);
        if(!strcmp(msgBuffer, "bluex")) {
            pixels.setPixelColor(2, pixels.Color(0, 0, 0));
            pixels.show();  // 여기서 변경된 부분
        }
        if(!strcmp(msgBuffer, "redx")) {
            pixels.setPixelColor(0, pixels.Color(0, 0, 0));
            pixels.show();  // 여기서 변경된 부분
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

    else if(!strcmp(topic, "TeamET/Lroom/buzzer/cmd")) {
        int i;
        for(i = 0; i < (int)length; i++) {
            msgBuffer[i] = payload[i];
        } 
        msgBuffer[i] = '\0';
        Serial.printf("\n%s -> %s", topic, msgBuffer);

        if(!strcmp(msgBuffer, "on")) {
            for (int j = 0; j < sizeof(song)/sizeof(int); j++) // 연주 길이만큼 숫자 조정
            {
                tone(PIEZO, song[j], TimeTime); // 배열순으로 노래 재생
                delay(TimeTime);
            }
        }
    }
}
