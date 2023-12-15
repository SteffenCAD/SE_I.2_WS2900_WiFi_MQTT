#include "main.h"

char WsDataString[200];

void setup() 
{
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(ComBaud);
  WsSerial.begin(WsBaud);

  Serial.println("connect to wifi");
  WiFi.begin(ssid, password);
  Serial.println("ntp begin");
  NtpClient.begin();
  NtpClient.setTimeOffset(0);

  Serial.println("router begin");
  WsRouter.begin(&WsBuffer, &WsData, &WsSerial, &NtpClient);
  NtpClient.update();
  Serial.println(NtpClient.getFormattedTime());
}

unsigned long millis_old = 0;
unsigned long millis_delay = 10;


void loop() 
{
  //Ntp client will update every 60 seconds
  NtpClient.update();

  //read all bytes to ring buffer
  while(WsSerial.available() > 0)
  {
    char input = WsSerial.read();
    WsBuffer.add(input);
  }

  //weather station routing and responses
  WsRouter.route();

  //if WsData has new data, print to debug Serial
  if(WsData.available())
  {
    //Jan send MQTT
    WsData.print(WsDataString);
    Serial.println(WsDataString);
  }
}
