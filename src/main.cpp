#include <Arduino.h>

#define PIR 16
#define LED 15

void setup()
{
  Serial.begin(115200);

  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);
}

void loop()
{
  int value = digitalRead(PIR);

  Serial.println(value);

  if(value == HIGH)
  {
    Serial.println("There is a human!!!");
    digitalWrite(LED, HIGH);
  }
  else
  {
    Serial.println("Nobody is detected.");
    digitalWrite(LED, LOW);
  }

  delay(500);
}