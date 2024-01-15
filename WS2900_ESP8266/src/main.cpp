#include "main.h"

Ticker timer(cyclicUpdate,5000);

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
  
  DbgSerial.println("--------------------------------------------------------");
  DbgSerial.println("start WS2900");

  //OTA-Update stuff
  DbgSerial.println("read rtcUserMemory for update mode");
  ESP.rtcUserMemoryRead(otaEnalbedOff, &otaEnabled, sizeof(otaEnabled));
  
  DbgSerial.println("read user settings from eeprom");
  settings.begin(&DbgSerial);
  settings.load();

  DbgSerial.println("DBG | NTP | init");
  NtpClient.setPoolServerName(settings.ntp_server.c_str());
  NtpClient.begin();
  NtpClient.setTimeOffset(0);

  DbgSerial.println("DBG| WsBuffer | init buffer");
  WsBuffer.begin(&WsSerial);

  DbgSerial.println("DBG | WsRouter | init");
  WsRouter.begin(&WsBuffer, &WsData, &WsSerial, &NtpClient, &DbgSerial);

  DbgSerial.println("DBG | HTTP | init server");
  server.on("/", HTTP_GET,[](){server.send(200, "text/html", htmlUpdateMode);});
  server.begin();

  timer.start();

  
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

    DbgSerial.println("DBG| create access point");
    createAp();

    server.on("/reset", HTTP_GET, []() { 
    ESP.restart();
    server.send(200, "text/plain", "WS zurückgesetzt");
    });

    DbgSerial.println("DBG| begin elegant ota");
    ElegantOTA.begin(&server);    // Start ElegantOTA

    DbgSerial.println("DBG| begin settings");
    settings.open(&server);
  }
  else
  {
    DbgSerial.println("DBG| Enter Normal mode");
    connectToWifi();
  }

  DbgSerial.println("Init complete");
}

/// @brief main loop
void loop() 
{
  timer.update();
  server.handleClient();


  if(!otaRunning)
  {
    //Ntp client will update every 60 seconds
    NtpClient.update();

    //execute buffer
    WsBuffer.loop();

    //weather station routing and responses
    WsRouter.route();

    //if WsData has new data, print to debug Serial
    if(WsData.available()){sendMqttData();}

    //update mqtt client
    if (mqttEnabled == true){ MqttClient.loop();}
      
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
    ElegantOTA.loop();

    if(initOtaTime != 0 && (initOtaTime + initOtaTimeout) < millis())
    {
      otaRunning = false;
      initOtaTime = 0;

      DbgSerial.println("DBG| Reset otaEnabled flag in RTC storage");
      otaEnabled = 0;
      ESP.rtcUserMemoryWrite(otaEnalbedOff, &otaEnabled, sizeof(otaEnabled));

      DbgSerial.println("DBG| Reset esp8266");
      ESP.restart();
    }
  }
}

void connectToWifi() 
{
  DbgSerial.println("WIFI|  Connect to");

  DbgSerial.print("WIFI|  SSID: ");
  DbgSerial.println(settings.wifiSsid.c_str());
  DbgSerial.print("WIFI|  PASS: ");
  DbgSerial.println(settings.wifiPwd.c_str());

  WiFi.mode(WIFI_STA);
  WiFi.begin(settings.wifiSsid.c_str(), settings.wifiPwd.c_str());

  //set autoconnect
  WiFi.setAutoConnect(true);
  WiFi.persistent(true);

  NtpClient.update();
}

void connectMqtt()
{
  if (mqttEnabled == true)
  {
    DbgSerial.println("MQTT|Init");    
    DbgSerial.println(settings.mqtt_server);
    DbgSerial.println(settings.mqtt_port);

    DbgSerial.println(settings.mqtt_clientId);
    DbgSerial.println(settings.mqtt_user);
    DbgSerial.println(settings.mqtt_pass);

    DbgSerial.println("MQTT|SetInsecure"); 
    espClientSec.setInsecure();

    DbgSerial.println("MQTT|reduce buffer size on TLS"); 
    espClientSec.setBufferSizes(1024,1024);

    DbgSerial.println("MQTT|SetServer"); 
    MqttClient.setServer(settings.mqtt_server.c_str(), settings.mqtt_port);

    DbgSerial.println("MQTT|connect"); 
    MqttClient.connect(settings.mqtt_clientId.c_str(), settings.mqtt_user.c_str(), settings.mqtt_pass.c_str());

    DbgSerial.print("MQTT| SSL error: ");
    char err_buf[256];
    espClientSec.getLastSSLError(err_buf, sizeof(err_buf));
    DbgSerial.println(err_buf);

  }
  else
  {
    DbgSerial.println("MQTT disabled in main.h");
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
}

void cyclicUpdate()
{
  DbgSerial.println("------------------------------------");
  DbgSerial.print("DBG| current TIME: ");
  DbgSerial.println(NtpClient.getFormattedTime());

  DbgSerial.print("DBG| current IP: ");
  DbgSerial.println(WiFi.localIP());

  DbgSerial.print("DBG| free Heap ");
  DbgSerial.println(ESP.getFreeHeap());

  //do wifi stuff
  if (WiFi.isConnected())
  {
    DbgSerial.println("DBG| WiFI: CONNECTED");
    WsRouter.setState(WsState::WiFiCon);
  }
  else
  {
    DbgSerial.println("DBG| WiFI: DISCONNECTED");
    WsRouter.setState(WsState::WiFiDiscon);  
  }

  //mqtt stuff
  if(WiFi.isConnected() && !MqttClient.connected())
  {
    connectMqtt();
  }
  DbgSerial.println("------------------------------------");
}

void sendMqttData()
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
        DbgSerial.println(settings.mqtt_topic);
        DbgSerial.println(JsonData);
        MqttClient.publish(settings.mqtt_topic.c_str(), arr);
      }
      else
      {
        connectMqtt();
      }
    }
  }
}