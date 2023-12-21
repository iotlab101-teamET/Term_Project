#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ConfigPortal8266.h>
#include <Arduino.h>
#include <Wire.h>

#define flame 16

char*               ssid_pfix = (char*)"firere";
String              user_config_html = ""
    "<p><input type='text' name='mqttServer' placeholder='mqtt Broker'>";
 
const char*         mqttServer;
const int           mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

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
    while (!client.connected()) {
        Serial.println("Connecting to MQTT...");
 
        if (client.connect("firerere")) {
            Serial.println("connected");  
        } else {
            Serial.print("failed with state "); Serial.println(client.state());
            delay(2000);
        }
    }
}

void loop() {
  client.loop();

  int state = digitalRead(flame);

  char buf[10];   
  sprintf(buf, "%d", state);
  Serial.printf("%d\n", state);
  client.publish("TeamET/Lroom/sensor/evt/fire", buf);
  
  delay(2000);
}
