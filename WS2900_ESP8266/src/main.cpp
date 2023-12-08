#include "main.h"

void setup() 
{
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(ComBaud);

  WsSerial.begin(WsBaud);
  WsRouter.begin(&WsBuffer, &WsData, &WsSerial);
}


void loop() 
{
  //read all bytes to ring buffer
  while(WsSerial.available() > 0)
  {
    WsBuffer.add(WsSerial.read());
  }

  //weather station routing
  WsRouter.route();

  //if WsData has new data, print to debug Serial
  if(WsData.available())
  {
    char data[200];
    WsData.print(data);
    Serial.println(data);
  }
}
