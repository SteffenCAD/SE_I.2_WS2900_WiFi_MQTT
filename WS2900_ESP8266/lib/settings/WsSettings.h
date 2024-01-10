#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "Arduino.h"
#include "EEPROM.h"
#include "ESP8266WiFi.h" 
#include "WiFiClient.h"
#include "ESP8266WebServer.h"
#include "ArduinoJson.h"
#include "SoftwareSerial.h"

#define eepromStartAddr 0   
#define eepromSize 512      //maximum size of eeprom

class WsSettings
{
private:
    //struct to serialize settings to eeprom
    struct data
    {
        char      wifiSsid[20];
        char      wifiPwd [20];
        char      mqtt_server[100];
        uint16_t  mqtt_port;
        char      mqtt_clientId[20];
        char      mqtt_user[20];
        char      mqtt_pass [20];
        char      mqtt_topic[100];
        char      ntp_server[100];
    };    

    ESP8266WebServer *webServer;
    SoftwareSerial *DbgSerial;

    const char *htmlCode = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 20px;
      background-color: #18191A;
      color: #E4E6EB;
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    form {
      text-align: left;
      width: 300px; /* Set a specific width for the form */
      margin-top: 20px; /* Add some top margin for spacing */
    }
    input {
      margin: 10px;
      width: calc(100% - 20px); /* Set the width of the input fields to be 100% minus margins */
    }
    #saveButton {
      padding: 10px 20px;
      font-size: 16px;
      cursor: pointer;
    }
    #message {
      margin-top: 15px;
      font-size: 18px;
    }
    hr {
      border: 1px solid #E4E6EB;
      margin: 20px 0;
      width: 100%; /* Make the horizontal rule span the entire width */
    }
  </style>
</head>
<body>
  <h2>WS2900 Einstellungen</h2>
  <form>
    <hr>
    WLAN SSID:      <input type="text" id="wlanSsid" name="wlanSsid"><br>
    WLAN Password:  <input type="password" id="wlanPass" name="wlanPass"><br>
    <hr>
    MQTT server:    <input type="text" id="mqttServer" name="mqttServer"><br>
    MQTT port:      <input type="number" id="mqttPort" name="mqttPort" min="0" max="65535"><br>
    MQTT client ID: <input type="text" id="mqttClId" name="mqttClId"><br>
    MQTT user:      <input type="text" id="mqttUser" name="mqttUser"><br>
    MQTT Password:  <input type="password" id="mqttPass" name="mqttPass"><br>
    MQTT topic:     <input type="text" id="mqttTopic" name="mqttTopic"><br>
    <hr>
    NTP server:     <input type="text" id="ntpServer" name="ntpServer"><br>
    <hr>
    <input type="button" value="Save" id="saveButton" onclick="saveSettings()">
    <hr>
  </form>
  <div id="message"></div>

  <script>
    window.onload = function() {
      loadSettings();
    };

    function saveSettings() {
      var wlanPass = document.getElementById("wlanPass").value;
      var wlanSsid = document.getElementById("wlanSsid").value;
      var mqttServer = document.getElementById("mqttServer").value;
      var mqttPort = document.getElementById("mqttPort").value;
      var mqttClId = document.getElementById("mqttClId").value;
      var mqttUser = document.getElementById("mqttUser").value;
      var mqttPass = document.getElementById("mqttPass").value;
      var mqttTopic = document.getElementById("mqttTopic").value;
      var ntpServer = document.getElementById("ntpServer").value;

      var url = "/save?wlanPass=" + wlanPass + "&wlanSsid=" + wlanSsid + "&mqttServer=" + mqttServer + "&mqttPort=" + mqttPort + "&mqttClId=" + mqttClId + "&mqttUser=" + mqttUser + "&mqttPass=" + mqttPass + "&mqttTopic=" + mqttTopic+ "&ntpServer=" + ntpServer;

      var xhr = new XMLHttpRequest();
      xhr.open("GET", url, true);
      xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
          document.getElementById("message").innerHTML = "Einstellungen gespeichert";
          loadSettings();
        }
      };
      xhr.send();
    }

    function loadSettings() {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/load", true);
      xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
          var settings = JSON.parse(xhr.responseText);
          document.getElementById("wlanSsid").value = settings.wlanSsid;
          document.getElementById("mqttServer").value = settings.mqttServer;
          document.getElementById("mqttPort").value = settings.mqttPort;
          document.getElementById("mqttClId").value = settings.mqttClId;
          document.getElementById("mqttUser").value = settings.mqttUser;
          document.getElementById("mqttTopic").value = settings.mqttTopic;
          document.getElementById("ntpServer").value = settings.ntpServer;
        }
      };
      xhr.send();
    }
  </script>
</body>
</html>
    )html";


    void saveToEEPROM(const char *password, const char *ssid, const char *topic);


public:
    void begin(SoftwareSerial *serial);

    void load();
    void save();

    void open(ESP8266WebServer *server);
    void close();

    String      wifiSsid        = "Hotspot";
    String      wifiPwd         = "test";
    String      mqtt_server     = "se-homecontrol.informatik.tha.de";
    uint16_t    mqtt_port       = 15;
    String      mqtt_clientId   = "Noerdlingen_11";
    String      mqtt_user       = "Noerdlingen_1";
    String      mqtt_pass       = "Ryn6Kj1MDG";
    String      mqtt_topic      = "se/i2projekt/noerdlingen/wetterstation";
    String      ntp_server      = "time.fh-augsburg.de";

};

#endif
