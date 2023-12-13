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

//##    define stuff    ####
#define LED_PIN 4

#define WsTxPin D3
#define WsRxPin D2
#define WsBaud 9600

#define ComBaud 115200

#define bufSize 300

const char *ssid     = "REPLACE_WITH_YOUR_SSID";
const char *password = "REPLACE_WITH_YOUR_PASSWORD";

//##    objects         ####    
Ws2900Data      WsData;
SoftwareSerial  WsSerial(WsRxPin, WsTxPin); // RX, TX
router          WsRouter;
ringbuffer      WsBuffer;

WiFiUDP         NtpUdp;
NTPClient       NtpClient(NtpUdp, "pool.ntp.org");



#endif