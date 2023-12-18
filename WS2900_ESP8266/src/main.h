#ifndef _MAIN_H_
#define _MAIN_H_

//##    include stuff   ####
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
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>


//##    define stuff    ####
#define LED_PIN 4

#define WsTxPin D3
#define WsRxPin D2
#define WsBaud 9600

#define ComBaud 115200

#define bufSize 300

unsigned long initOtaTime = 0;
long long int initOtaTimeout = 3600000;
//unsigned long initOtaTimeout = 60000;

const char *ssid     = "Hotspot";
const char *password = "Passwort";

//define MQTT stuff
const char*     mqtt_server     = "se-homecontrol.informatik.tha.de";
const uint16_t  mqtt_port       = 15;
const char*     mqtt_clientId   = "Noerdlingen_11";
const char*     mqtt_user       = "Noerdlingen_1";
const char*     mqtt_pass       = "Ryn6Kj1MDG";

const char* mqtt_topic          = "se/i2projekt/noerdlingen/wetterstation";
const char* mqtt_topic2         = "se/i2projekt/noerdlingen/WS";
//##    objects         ####    
Ws2900Data      WsData;
SoftwareSerial  WsSerial(WsRxPin, WsTxPin); // RX, TX
router          WsRouter;
ringbuffer      WsBuffer;

WiFiUDP         NtpUdp;
NTPClient       NtpClient(NtpUdp, "time.fh-augsburg.de");

WiFiClientSecure    espClientSec;
PubSubClient        MqttClient(espClientSec);

AsyncWebServer server(80);

#endif 