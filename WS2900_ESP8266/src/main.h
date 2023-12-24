#ifndef _MAIN_H_
#define _MAIN_H_
//https://patorjk.com/software/taag/#p=display&h=0&v=3&f=Bigfig&t=

//-------------------------------
// ___    __       __ _ 
//  | |\|/  |  | ||_ | \
// _|_| |\__|__|_||__|_/
//-------------------------------
#include "Arduino.h"
#include "Ws2900Data.h"
#include "SoftwareSerial.h"
#include "ringbuffer.h"
#include "router.h"
#include "ESP8266WiFi.h"
#include "NTPClient.h"
#include "WiFiUdp.h"

#include "PubSubClient.h"

#include <ESPAsyncTCP.h>
#include <Ticker.h>
#include <ESP8266WebServer.h>
#include <ElegantOTA.h>

//-------------------------------
//  _  __ _____   ______ _     __
// | \|_ |_  | |\| |  | / \|\|(_ 
// |_/|__|  _|_| |_|_ | \_/| |__)
//-------------------------------
#define LED_PIN 4
#define OTA_PIN 14

#define WsTxPin 
#define WsRxPin 
#define WsBaud  9600

#define DbgTxPin    12 
#define DbgRxPin    16 //not used only debug
#define DbgBaud     115200

#define bufSize 300

//-------------------------------
//  _  _  _ ___ _ ___\ / _  __
// |_)|_)/ \ | / \ |  Y |_)(_ 
// |  | \\_/ | \_/ |  | |  __)
//-------------------------------
void connectToWifi();
void onWifiConnect(const WiFiEventStationModeGotIP& event);
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event);
void createAp();
//-------------------------------
//    ___ __ __
// |V| | (_ /  
// | |_|___)\__
//-------------------------------
long long int serialTimestamp = 0;


//-------------------------------
//    ___ _____
// | | | |_  | 
// |^|_|_|  _|_
//-------------------------------
const char *ssid     = "Hotspot";
const char *password = "Passwort";


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

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

String otaMessage = "";

uint32_t otaEnabled = 0;
#define otaEnalbedOff 0

//-------------------------------
//     _ ______
// |V|/ \ |  | 
// | |\_X |  | 
//-------------------------------
const char*     mqtt_server     = "se-homecontrol.informatik.tha.de";
const uint16_t  mqtt_port       = 15;
const char*     mqtt_clientId   = "Noerdlingen_11";
const char*     mqtt_user       = "Noerdlingen_1";
const char*     mqtt_pass       = "Ryn6Kj1MDG";
const char*     mqtt_topic      = "se/i2projekt/noerdlingen/wetterstation";
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

WiFiUDP         NtpUdp;
NTPClient       NtpClient(NtpUdp, "time.fh-augsburg.de");

WiFiClientSecure    espClientSec;
PubSubClient        MqttClient(espClientSec);

ESP8266WebServer server(80);


#endif 