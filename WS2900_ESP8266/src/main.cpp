#include "main.h"

void setup() 
{
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(ComBaud);
  WsSerial.begin(WsBaud);

  Serial.println("connect to wifi");
  espClientSec.setInsecure();   //allow unsecure connections (don't compare certificate)
  WiFi.begin(ssid, password);

  Serial.println("ntp begin");
  NtpClient.begin();
  NtpClient.setTimeOffset(0);

  Serial.println("router begin");
  WsRouter.begin(&WsBuffer, &WsData, &WsSerial, &NtpClient);
  NtpClient.update();

  Serial.println("init MQTT");
  MqttClient.setServer(mqtt_server, mqtt_port);
  MqttClient.connect(mqtt_clientId, mqtt_user, mqtt_pass);
}

void loop() 
{
  //Ntp client will update every 60 seconds
  NtpClient.update();

  //update mqtt client
  MqttClient.loop();

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
    //convert string to char array for publishing
    String JsonData = WsData.toJson(true);
    char arr[JsonData.length()+1];
    JsonData.toCharArray(arr, JsonData.length()+1);

    //print weather data to debug console
    Serial.println(WsData.toString());

    Serial.println(WiFi.localIP());

    //publish to mqtt
    if(MqttClient.connected())
    {
      MqttClient.publish(mqtt_topic, arr);
    }
    else
    {
      MqttClient.connect(mqtt_clientId, mqtt_user, mqtt_pass);
    }
  }
}
