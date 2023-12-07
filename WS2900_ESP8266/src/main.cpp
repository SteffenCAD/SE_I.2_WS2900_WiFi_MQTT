#include "main.h"


void setup() 
{
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(ComBaud);

  WsSerial.begin(WsBaud);
  WsBuffer.begin();
  WsRouter.begin(&WsBuffer, &WsData);
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
    Serial.println(WsData.print());
  }

}
