#include "main.h"

char WsDataString[200];

void setup() 
{
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(ComBaud);
  Serial.println("Hello to weatherstation 2900");
  WsSerial.begin(WsBaud);
  WsRouter.begin(&WsBuffer, &WsData, &WsSerial);
}

unsigned long millis_old = 0;
unsigned long millis_delay = 10;


void loop() 
{

  //read all bytes to ring buffer
  while(WsSerial.available() > 0)
  {
    char input = WsSerial.read();
    WsBuffer.add(input);
  }

  //weather station routing
  WsRouter.route();

  //if WsData has new data, print to debug Serial
  if(WsData.available())
  {
    //Jan sendet MQTT

    WsData.print(WsDataString);
    Serial.println(WsDataString);
  }
}
