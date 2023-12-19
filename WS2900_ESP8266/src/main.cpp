#include "main.h"





void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  Serial.println(WiFi.localIP());

  Serial.println("Connect to NTP");
  NtpClient.begin();
  NtpClient.setTimeOffset(0);

  NtpClient.update();
  Serial.println(NtpClient.getFormattedTime());

  if (mqttEnabled == true)
  {
    Serial.println("init MQTT");
    MqttClient.setServer(mqtt_server, mqtt_port);
    MqttClient.connect(mqtt_clientId, mqtt_user, mqtt_pass);
  }
  else
  {
    Serial.println("MQTT disabled in main.h");
  }

  Serial.println("pre init OTA");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", otaMessage);
    });
  otaMessage = "WS is in normal operation mode. For updating the firmware please enter OTA update mode and connect to the OTA Hotspot";
  server.begin();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  wifiReconnectTimer.once(2, connectToWifi);
}












void setup() 
{
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(ComBaud);
  WsSerial.begin(WsBaud);

  Serial.println();
  espClientSec.setInsecure();   //allow unsecure connections (don't compare certificate)



  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  connectToWifi();
  

  Serial.println("WS Router begin");
  WsRouter.begin(&WsBuffer, &WsData, &WsSerial, &NtpClient);


  Serial.println("Init complete");
}

void loop() 
{

  if (WiFi.isConnected())
  {
    //Ntp client will update every 60 seconds
    NtpClient.update();

    if (serialTimestamp + 10000 < millis()){
        Serial.print(NtpClient.getFormattedTime());
        Serial.print("  ");
        Serial.println(WiFi.localIP());
        serialTimestamp = millis();
    }


    //update mqtt client
    if (mqttEnabled == true)
    {
      MqttClient.loop();
    }
  }

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


    if (WiFi.isConnected())
    {

      if (mqttEnabled == true)
      {
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
  }




  //if ota service has to init
  if(WsRouter.initOTA() || (otaRunning == false && millis() > 120000))
  {
    Serial.println("OTA requested");
    initOtaTime = millis();

    //close WiFi
    Serial.println("Disconnect WiFi");

    wifiDisconnectHandler = NULL;

    if (WiFi.isConnected())
    {
      WiFi.disconnect(true);
      while (WiFi.status() == WL_CONNECTED) {
        delay(500);
        Serial.println("Wait for disconnect...");
      } 
    }

    Serial.println("Start WiFi OTA hotspot");

    otaMessage = "WS is in OTA update mode. For firmware update please go to http://192.168.1.1/update";

    //setup hotspot
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(hotSpotSsid, hotSpotPassword);

    //init OTA server
    AsyncElegantOTA.begin(&server);    // Start ElegantOTA
    Serial.println("OTA started");
    otaRunning = true;
  }



  if(initOtaTime != 0 && (initOtaTime + initOtaTimeout) < millis())
  {
    otaRunning = false;
    initOtaTime = 0;
    Serial.println("Reset esp8266");
    ESP.restart();
  }



}
