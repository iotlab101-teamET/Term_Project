#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <ConfigPortal8266.h>
#include <Servo.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <SSD1306.h>

char*               ssid_pfix = (char*)"SJ";
String              user_config_html = ""
    "<p><input type='text' name='mqttServer' placeholder='mqtt Broker'>";

const char*         mqttServer;
const int           mqttPort = 1883;

unsigned long       pubInterval = 2000;
unsigned long       lastPublished = - pubInterval;

WiFiClient espClient;
PubSubClient client(espClient);
void callback(char* topic, byte* payload, unsigned int length);

Servo myservo;
int pos = 0;

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA); 
    loadConfig();
    
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
 
        if (client.connect("Lroom-B")) {
            Serial.println("connected");  
        } else {
            Serial.print("failed with state "); Serial.println(client.state());
            delay(2000);
        }
    }
    myservo.attach(15);  // 15번 핀으로 PWM 사용
    client.subscribe("TeamET/Lroom/servo/cmd");
}

void loop() {
    client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
 
    char msgBuffer[20];

    Serial.println("servo on!");

    if(!strcmp(topic, "TeamET/Lroom/servo/cmd")) {
      int i;
      for(i = 0; i < (int)length; i++) 
      {
          msgBuffer[i] = payload[i];
      } 
      msgBuffer[i] = '\0';
      Serial.printf("\n%s -> %s", topic, msgBuffer);

      if(!strcmp(msgBuffer, "open")) 
      {
        pos = 180;
        myservo.write(pos);
        Serial.println("on");
      }
      if(!strcmp(msgBuffer, "close")) 
      {
        pos = 0;
        myservo.write(pos);
        Serial.println("off");
      }
    }
}
