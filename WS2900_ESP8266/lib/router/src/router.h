#ifndef _ROUTER_H_
#define _ROUTER_H_

#include "Arduino.h"
#include "ringbuffer.h"
#include "Ws2900Data.h"
#include "SoftwareSerial.h"
#include "NTPClient.h"
#include "time.h"

#define bufSize 300

enum WsState {WiFiCon, WiFiDiscon, UpdateMode};

class router
{

private:
    ringbuffer      *Wsbuffer;
    Ws2900Data      *WsData;
    HardwareSerial  *WsSerial;
    NTPClient       *NtpClient;
    SoftwareSerial  *DbgSerial;
    
    bool initOta = false;
    WsState currentState = WsState::WiFiDiscon;

public:
    /// @brief init the router class
    /// @param buffer 
    /// @param data 
    /// @param serial 
    void begin(ringbuffer *buffer, Ws2900Data *data, HardwareSerial *serial, NTPClient *ntpClient, SoftwareSerial *DBGserial);
    
    /// @brief has to execute in main loop to process data
    void route();

    /// @brief add checksum at last position of array
    /// @param array 
    void checkSum_8Mod256(char *array, size_t size);

    /// @brief create array of date in pattern of WS2900;
    /// @param array 
    void createDate(char *array);

    /// @brief 
    /// @return 
    bool initOTA();

    void setState(WsState state);
};


#endif