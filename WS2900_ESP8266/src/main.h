#ifndef _MAIN_H_
#define _MAIN_H_

//https://patorjk.com/software/taag/#p=display&h=0&v=3&f=Bigfig&t=
//-------------------------------
/* ___    __       __ _         */
/*  | |\|/  |  | ||_ | \        */
/* _|_| |\__|__|_||__|_/        */
//-------------------------------
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "ESP8266WiFi.h"
#include "NTPClient.h"
#include "WiFiUdp.h"
#include "Ticker.h"

#include "Ws2900Data.h"
#include "ringbuffer.h"
#include "router.h"
#include "WsSettings.h"

#include "PubSubClient.h"

#include <ESP8266WebServer.h>
#include <ElegantOTA.h>


//-------------------------------
//  _  __ _____   ______ _     __
// | \|_ |_  | |\| |  | / \|\|(_ 
// |_/|__|  _|_| |_|_ | \_/| |__)
//-------------------------------
#define LED_PIN     4
#define OTA_PIN     14

#define WsTxPin     0
#define WsRxPin     0
#define WsBaud      9600

#define DbgTxPin    12 
#define DbgRxPin    16 //not used only debug
#define DbgBaud     115200

#define bufSize     300

//-------------------------------
//  _  _  _ ___ _ ___\ / _  __
// |_)|_)/ \ | / \ |  Y |_)(_ 
// |  | \\_/ | \_/ |  | |  __)
//-------------------------------
void connectToWifi();
void createAp();
void connectMqtt();
void cyclicUpdate();
void sendMqttData();
//-------------------------------
//    ___ __ __
// |V| | (_ /  
// | |_|___)\__
//-------------------------------
long long int serialTimestamp = 0;

//-------------------------------
//  _ ___ _        _  _  _ ___ __
// / \ | |_|---| ||_)| \|_| | |_ 
// \_/ | | |   |_||  |_/| | | |__
//-------------------------------
const char* ApSsid      = "WS-OTA-Hotspot";
const char* ApPassword  = "123456789";

IPAddress local_IP(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

unsigned long initOtaTime = 0;
unsigned long initOtaTimeout = 3600000;
bool otaRunning = false;

uint32_t otaEnabled = 0;
#define otaEnalbedOff 0

//-------------------------------
//     _ ______
// |V|/ \ |  | 
// | |\_X |  | 
//-------------------------------
const bool      mqttEnabled     = true;

//-------------------------------
//  _  _     __ _____ __
// / \|_)  ||_ /   | (_ 
// \_/|_)\_||__\__ | __)
//-------------------------------

Ws2900Data      WsData;

HardwareSerial  WsSerial(0);
SoftwareSerial  DbgSerial(DbgRxPin, DbgTxPin);

router          WsRouter;
ringbuffer      WsBuffer;
WsSettings      settings;

WiFiUDP         NtpUdp;
NTPClient       NtpClient(NtpUdp);

WiFiClientSecure    espClientSec;
PubSubClient        MqttClient(espClientSec);

ESP8266WebServer server(80);

#pragma region defaultHTML
const char * const htmlUpdateMode = R"html(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>WS2900 Update Modus</title>
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
        h2 {
            text-align: center;
        }
        hr {
            border: 1px solid #E4E6EB;
            margin: 20px 0;
            width: 100%; /* Make the horizontal rule span the entire width */
        }
        a {
            color: #3498db; /* Set the hyperlink color */
            text-decoration: none;
        }
        a:hover {
            color: #2077b9; /* Set the hyperlink color on hover */
            text-decoration: underline;
        }
        .button {
            padding: 10px 0; /* Adjust padding to center vertically within the button */
            width: 100%; /* Make the button as wide as the container */
            font-size: 16px;
            cursor: pointer;
        }
    </style>
</head>
<body>
    <form>
        <h2>WS2900</h2>
        <hr>
        <p>Softwareversion: <span id="softwareVersion">1.0.0</span></p>
        <hr>
        <p><a href="http://192.168.1.100/update" onclick="window.location.href = 'http://192.168.1.100/update'; return false;">Update</a></p>
        <p><a href="http://192.168.1.100/settings" onclick="window.location.href = 'http://192.168.1.100/settings'; return false;">Einstellungen</a></p>
        <hr>
        <button class="button" onclick="resetWS()">Reset WS</button>
    </form>
    <script>
        function resetWS() {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/reset", true);
            xhr.send();
        }
    </script>
</body>
</html>
    )html";
#pragma endregion

#endif 