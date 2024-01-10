#include "WsSettings.h"

void WsSettings::begin(SoftwareSerial *serial)
{
  //store dbg serial
  DbgSerial = serial;
}

void WsSettings::open(ESP8266WebServer *wserver)
{
  //store webserver reference in class
  this->webServer = wserver;

  webServer->on("/settings", HTTP_GET, [&]()
  {   
    webServer->send(200, "text/html", htmlCode);
  });

  webServer->on("/save", HTTP_GET, [&]() 
  {
    wifiSsid        = webServer->arg("wlanSsid");
    //make sure, passwort is not empty
    String wifiPwd_temp = webServer->arg("wlanPass");
    if(wifiPwd_temp != ""){wifiPwd = wifiPwd_temp;}
    mqtt_server     = webServer->arg("mqttServer");
    //convert string to int
    String mqtt_port_str = webServer->arg("mqttPort");
    mqtt_port       = (uint16_t)mqtt_port_str.toInt();
    mqtt_clientId   = webServer->arg("mqttClId");
    mqtt_user       = webServer->arg("mqttUser");
    //make sure, passwort is not empty
    String mqtt_pass_temp = webServer->arg("mqttPass");
    if(mqtt_pass_temp != ""){mqtt_pass = mqtt_pass_temp;}
    mqtt_topic      = webServer->arg("mqttTopic");
    ntp_server      = webServer->arg("ntpServer");
    //save data to eeprom
    save();

    webServer->send(200, "text/plain", "Einstellungen gespeichert");

  });

  webServer->on("/load", HTTP_GET, [this]() 
  {
    JsonDocument jsonDoc;
    jsonDoc["wlanSsid"]     = wifiSsid;
    //jsonDoc["wlanPass"]     = "";
    jsonDoc["mqttServer"]   = mqtt_server;
    jsonDoc["mqttPort"]     = mqtt_port;
    jsonDoc["mqttClId"]     = mqtt_clientId;
    jsonDoc["mqttUser"]     = mqtt_user;
    //jsonDoc["smqttPasssid"] = "";
    jsonDoc["mqttTopic"]    = mqtt_topic;
    jsonDoc["ntpServer"]    = ntp_server;

    String jsonString;
    serializeJson(jsonDoc, jsonString);
    
    webServer->send(200, "application/json", jsonString);
  });
}

void WsSettings::close()
{

}

void WsSettings::save()
{
  //cast data to struct
  data romData;
  wifiSsid.toCharArray(romData.wifiSsid, wifiSsid.length()+1);
  wifiPwd.toCharArray(romData.wifiPwd, wifiPwd.length()+1);
  mqtt_server.toCharArray(romData.mqtt_server, mqtt_server.length()+1);
  romData.mqtt_port = mqtt_port;
  mqtt_clientId.toCharArray(romData.mqtt_clientId, mqtt_clientId.length()+1);
  mqtt_user.toCharArray(romData.mqtt_user, mqtt_user.length()+1);
  mqtt_pass.toCharArray(romData.mqtt_pass, mqtt_pass.length()+1);
  mqtt_topic.toCharArray(romData.mqtt_topic, mqtt_topic.length()+1);
  ntp_server.toCharArray(romData.ntp_server, ntp_server.length()+1);

  EEPROM.begin(512);
  EEPROM.put(0, romData);
  EEPROM.commit();
  EEPROM.end();
}

void WsSettings::load()
{
  //
  data romData;
  EEPROM.begin(512);
  EEPROM.get(0, romData);
  EEPROM.end();

  wifiSsid    = String(romData.wifiSsid);
  wifiPwd     = String(romData.wifiPwd);
  mqtt_server = String(romData.mqtt_server);
  mqtt_port   = romData.mqtt_port;
  mqtt_clientId = String(romData.mqtt_clientId);
  mqtt_user   = String(romData.mqtt_user);
  mqtt_pass   = String(romData.mqtt_pass);
  mqtt_topic  = String(romData.mqtt_topic);
  ntp_server  = String(romData.ntp_server);
}

