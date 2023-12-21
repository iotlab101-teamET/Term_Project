#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ConfigPortal8266.h>
#include <Arduino.h>
#include <Wire.h>

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

#define PIEZO 16
#define PIR 15

// 징글벨 첫 소절 음계 : 알기 쉽게 다장조의 음계로 변환하여 처리
int song[] = {NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_G4, 0, // 솔미레도솔(쉼표)
              NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, 0, // 솔미레도라(쉼표)
              NOTE_A4, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_B4, 0, // 라파미레시(쉼표)
              NOTE_G5, NOTE_G5, NOTE_F5, NOTE_D5, NOTE_E5, NOTE_C5, 0}; // 솔솔파레미도(쉼표)

// 징글벨 첫 소절 연주시간
int TimeTime[] = {250, 250, 250, 250, 750, 250, // 8분음표 = 250ms
              250, 250, 250, 250, 750, 250,
              250, 250, 250, 250, 750, 250,
              250, 250, 250, 250, 500, 250, 250};

char*               ssid_pfix = (char*)"kys";
String              user_config_html = ""
    "<p><input type='text' name='mqttServer' placeholder='mqtt Broker'>";
 
const char*         mqttServer;
const int           mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
void callback(char* topic, byte* payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA); 
    loadConfig();
    pinMode(PIR, INPUT);
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
 
        if (client.connect("hidoor")) {
            Serial.println("connected");  
        } else {
            Serial.print("failed with state "); Serial.println(client.state());
            delay(2000);
        }
    }
    client.subscribe("TeamET/door/buzzer/cmd");
}

void loop() {
  client.loop();

  int human = digitalRead(PIR);

  char buf[10];   
  sprintf(buf, "%d", human);
  Serial.printf("%d\n", human);
  client.publish("TeamET/door/sensor/evt/human", buf);
  
  delay(2000);
}

void callback(char* topic, byte* payload, unsigned int length) {
 
  char msgBuffer[20];

  if(!strcmp(topic, "TeamET/door/buzzer/cmd")) {
    int i;
    for(i = 0; i < (int)length; i++) {
        msgBuffer[i] = payload[i];
    } 
    msgBuffer[i] = '\0';
    Serial.printf("\n%s -> %s", topic, msgBuffer);

    if(!strcmp(msgBuffer, "on")) {
      for (int j = 0; j < 25; j++) // 연주 길이만큼 숫자 조정
      {
        tone(PIEZO, song[j], TimeTime[j]); // 배열순으로 노래 재생
        delay(TimeTime[j]);
      }
    }
  }
}
