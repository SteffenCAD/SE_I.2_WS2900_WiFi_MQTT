#include "main.h"


/// @brief setup
void setup() 
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(OTA_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, HIGH);

  WsSerial.begin(WsBaud);
  DbgSerial.begin(DbgBaud);
  
  DbgSerial.println("--------------------------------------------------------");
  espClientSec.setInsecure();   //allow unsecure connections (don't compare certificate)

  //OTA-Update stuff
  ESP.rtcUserMemoryRead(otaEnalbedOff, &otaEnabled, sizeof(otaEnabled));
  if(otaEnabled == 1 || digitalRead(OTA_PIN) == 0)
  {
    DbgSerial.println("Enter OTA-Update mode");

    //save state to runtime var to hold in update session
    otaRunning = true;

    //get system time to reset ota after 1hour
    initOtaTime = millis();

    //reset rtc memory
    otaEnabled = 0;
    ESP.rtcUserMemoryWrite(otaEnalbedOff, &otaEnabled, sizeof(otaEnabled));

    createAp();
    server.on("/", []() { server.send(200, "text/plain", "Hi! This is ElegantOTA Demo.");});
    ElegantOTA.begin(&server);    // Start ElegantOTA
    server.begin();
    DbgSerial.println("HTTP server started");

  }
  else
  {
    DbgSerial.println("Enter Normal mode");
    connectToWifi();
  }
 
  DbgSerial.println("WS Router begin");
  WsRouter.begin(&WsBuffer, &WsData, &WsSerial, &NtpClient);

  DbgSerial.println("Init complete");
}


/// @brief main loop
void loop() 
{
if(!otaRunning)
  {
    if (WiFi.isConnected())
    {
      //Ntp client will update every 60 seconds
      NtpClient.update();

      if (serialTimestamp + 10000 < millis())
      {
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
    while(WsSerial.available() > 0 )
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
      DbgSerial.println(WsData.toString());


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
    if(WsRouter.initOTA())
    {
      //if we want to update controller, write state to rtc memory and restart esp
      otaEnabled = 1;
      ESP.rtcUserMemoryWrite(otaEnalbedOff, &otaEnabled, sizeof(otaEnabled));
      ESP.restart();
    }
  }
  else
  {
    //Handle WiFi stuff
    server.handleClient();
    ElegantOTA.loop();

    if(initOtaTime != 0 && (initOtaTime + initOtaTimeout) < millis())
    {
      otaRunning = false;
      initOtaTime = 0;

      DbgSerial.println("Reset otaEnabled flag in RTC storage");
      otaEnabled = 0;
      ESP.rtcUserMemoryWrite(otaEnalbedOff, &otaEnabled, sizeof(otaEnabled));

      DbgSerial.println("Reset esp8266");
      ESP.restart();
    }
  }
}


void connectToWifi() 
{
  DbgSerial.println("Connecting to Wi-Fi...");

  //wifiConnectHandler    = WiFi.onStationModeGotIP(onWifiConnect);
  //wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  WiFi.begin(ssid, password);


  DbgSerial.println("Connected to Wi-Fi.");
  DbgSerial.println(WiFi.localIP());

  DbgSerial.println("Connect to NTP");
  NtpClient.begin();
  NtpClient.setTimeOffset(0);

  NtpClient.update();
  DbgSerial.println(NtpClient.getFormattedTime());

  if (mqttEnabled == true)
  {
    DbgSerial.println("init MQTT");
    MqttClient.setServer(mqtt_server, mqtt_port);
    MqttClient.connect(mqtt_clientId, mqtt_user, mqtt_pass);
  }
  else
  {
    DbgSerial.println("MQTT disabled in main.h");
  }

  DbgSerial.println("pre init OTA");
  otaMessage = "WS is in normal operation mode. For updating the firmware please enter OTA update mode and connect to the OTA Hotspot";
  server.on("/", []() { server.send(200, "text/plain", otaMessage);});
  server.begin();

}

void createAp()
{
  if (WiFi.isConnected())
  {
    WiFi.disconnect(true);
    while (WiFi.status() == WL_CONNECTED) {
      delay(500);
      DbgSerial.println("Wait for disconnect...");
    } 
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(local_IP,gateway,subnet);
  WiFi.softAP(ApSsid, ApPassword);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) 
{
  DbgSerial.println("Connected to Wi-Fi.");
  DbgSerial.println(WiFi.localIP());

  DbgSerial.println("Connect to NTP");
  NtpClient.begin();
  NtpClient.setTimeOffset(0);

  NtpClient.update();
  DbgSerial.println(NtpClient.getFormattedTime());

  if (mqttEnabled == true)
  {
    DbgSerial.println("init MQTT");
    MqttClient.setServer(mqtt_server, mqtt_port);
    MqttClient.connect(mqtt_clientId, mqtt_user, mqtt_pass);
  }
  else
  {
    DbgSerial.println("MQTT disabled in main.h");
  }

  DbgSerial.println("pre init OTA");
  otaMessage = "WS is in normal operation mode. For updating the firmware please enter OTA update mode and connect to the OTA Hotspot";
  server.on("/", []() { server.send(200, "text/plain", otaMessage);});
  server.begin();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) 
{
  DbgSerial.println("Disconnected from Wi-Fi.");
  wifiReconnectTimer.once(2, connectToWifi);
}