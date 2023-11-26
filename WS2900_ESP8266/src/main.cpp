#include <Arduino.h>

#define LED_PIN 4

void setup() 
{
  pinMode(LED_PIN, OUTPUT);

}

void loop() 
{

  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  delay(500);

}
