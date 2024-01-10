#include "main.h"


/// @brief setup
void setup() 
{
  //setup GPIOs
  pinMode(LED_PIN, OUTPUT);
  pinMode(OTA_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, HIGH);

  //setup Serial
  WsSerial.begin(WsBaud);
  DbgSerial.begin(DbgBaud);
  
  //DbgSerial.println("--------------------------------------------------------");
  //DbgSerial.println("start WS2900");

  //OTA-Update stuff
  //DbgSerial.println("read rtcUserMemory for update mode");
  ESP.rtcUserMemoryRead(otaEnalbedOff, &otaEnabled, sizeof(otaEnabled));
  
  //DbgSerial.println("read user settings from eeprom");
  settings.begin(&DbgSerial);
  settings.load();

  //DbgSerial.println("Init NTP time service");
  NtpClient.setPoolServerName(settings.ntp_server.c_str());

  //DbgSerial.println("WS Router begin");
  WsRouter.begin(&WsBuffer, &WsData, &WsSerial, &NtpClient);

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

    //DbgSerial.println("create access point");
    createAp();

    //DbgSerial.println("begin elegant ota");
    ElegantOTA.begin(&server);    // Start ElegantOTA

    //DbgSerial.println("begin HTTP Server");
    server.begin();

    settings.open(&server);
  }
  else
  {
    //DbgSerial.println("Enter Normal mode");
    connectToWifi();
  }
  
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
        DbgSerial.print(NtpClient.getFormattedTime());
        DbgSerial.print("  ");
        DbgSerial.println(WiFi.localIP());
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
            DbgSerial.print("MQTT|publish to: ");
            DbgSerial.print(settings.mqtt_topic);
            //DbgSerial.println(JsonData);
            MqttClient.publish(settings.mqtt_topic.c_str(), arr);
          }
          else
          {
            connectMqtt();
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
  //DbgSerial.println("Connecting to Wi-Fi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(settings.wifiSsid.c_str(), settings.wifiPwd.c_str());

  //set autoconnect
  WiFi.setAutoConnect(true);
  WiFi.persistent(true);

  //DbgSerial.println("Connected to Wi-Fi: ");
  //DbgSerial.print("SSID: ");
  //DbgSerial.println(settings.wifiSsid.c_str());
  //DbgSerial.print("PASS: ");
  //DbgSerial.println(settings.wifiPwd.c_str());

  //DbgSerial.println("Connect to NTP");
  NtpClient.begin();
  NtpClient.setTimeOffset(0);
  NtpClient.update();

  connectMqtt();

  server.on("/", HTTP_GET, 
  [](){   
    server.send(200, "text/html", htmlUpdateMode);
  });
}

void connectMqtt()
{
  if (mqttEnabled == true)
  {
    //DbgSerial.println("MQTT|Init");    
    //DbgSerial.println(settings.mqtt_server);
    //DbgSerial.println(settings.mqtt_port);

    //DbgSerial.println(settings.mqtt_clientId);
    //DbgSerial.println(settings.mqtt_user);
    //DbgSerial.println(settings.mqtt_pass);

    //DbgSerial.println("MQTT|SetInsecure"); 
    espClientSec.setInsecure();
    //DbgSerial.println("MQTT|SetServer"); 
    MqttClient.setServer(settings.mqtt_server.c_str(), settings.mqtt_port);
    //DbgSerial.println("MQTT|connect"); 
    MqttClient.connect(settings.mqtt_clientId.c_str(), settings.mqtt_user.c_str(), settings.mqtt_pass.c_str());
    
    DbgSerial.println(ESP.getFreeHeap());
    //char err_buf[256];
    //espClientSec.getLastSSLError(err_buf, sizeof(err_buf));
    //DbgSerial.print("SSL error: ");
    //DbgSerial.println(err_buf);
  }
  else
  {
    //DbgSerial.println("MQTT disabled in main.h");
  }
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

  //server.on("/", HTTP_GET, 
  //[](){   
  //  server.send(200, "text/html", htmlUpdateMode);
  //});
}
