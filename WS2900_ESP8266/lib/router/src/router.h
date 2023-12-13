#ifndef _ROUTER_H_
#define _ROUTER_H_

#include "Arduino.h"
#include "ringbuffer.h"
#include "Ws2900Data.h"
#include "SoftwareSerial.h"
#include "NTPClient.h"

#define bufSize 300

#define printRawData

    char temp1[4]; 
    char WsRawData[128]; //define array with size of raw weather data, exclude checksum

    //define patterns
    //pattern1 ################################################################################################################################################
    // status response
    char patt1[]        = {0x07,0x00,0x04,0x0B};
    //char resp1[]        = {0x07,0x00,0x06,0x04,0x03,0x14};    //no connection to WLAN, WLAN host ?
    //char resp1[]        = {0x07,0x00,0x06,0x01,0x02,0x10};    //only WLAN-symbol is blinking
    char resp1[]        = {0x07,0x00,0x06,0x02,0x02,0x11};      //connection to WLAN
    bool resp1_send     = false;
    unsigned long resp1_time    = 0;    //save time when response was received
    unsigned long resp1_delay   = 9;    //10ms

    //pattern2  ################################################################################################################################################
    //timezone (Europe/Berlin)
    char patt2[]        = {0x22, 0x00, 0x04, 0x26};
    //char resp2[]        = {0x22,0x00,0x2E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    //                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x46};
    char resp2[]        = {0x22,0x00,0x2E,0x00,0x00,0x0E,0x10,0x00,0xFF,0x07,0x38,0x10,0x15,0x45,0x75,0x72,0x6F,0x70,0x65,0x2F,0x42,0x65,0x72,0x6C,0x69,0x6E,
                                0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCC};
    bool resp2_send     = false;
    ulong resp2_time    = 0;    //save time when response was received
    ulong resp2_delay   = 9;    //10ms

    //pattern3  ################################################################################################################################################
    //time response 2
    char patt3[]        = {0x06, 0x00, 0x04, 0x0A};
    char resp3[]        = {0x06,0x00,0x0B,0x17,0x0C,0x09,0x14,0x38,0x2E,0x00,0xB7};
    //byte0:
    //byte1:
    //byte2:
    //byte3:    year    //all values as decimal
    //byte4:    month
    //byte6:    day 
    //byte7:    hour
    //byte8:    minute
    //byte9:    second
    //byte10:   -
    //byte11:   checksum
    bool resp3_send     = false;
    ulong resp3_time    = 0;    //save time when response was received
    ulong resp3_delay   = 9;    //10ms

    //pattern5  ################################################################################################################################################
    // ???
    //##########################################################################################################################################################
    char patt5[]        = {0x20,0x00,0x04,0x24};
    char resp5[]        = {0x20,0x00,0x0A,0x34,0x94,0x54,0x96,0x1F,0x52,0x4D};

    bool resp5_send     = false;
    ulong resp5_time    = 0;    //save time when response was received
    ulong resp5_delay   = 9;    //20ms

    //pattern4  ################################################################################################################################################
    // weather data
    //##########################################################################################################################################################
    //char patt4[]        = {0x4C,0x09,0x00,0x00,0x00,0x00,0x7A,0x04,0x00};
    char patt4[]        = {0x4C,0x09,0x00,0x00};
    char resp4[]        = {0x01,0x00,0x07,0x01,0x04,0x04,0x11,0x01,0x00,0x09,0x01,0x04,0x04,0x04,0x04,0x1B};

    bool resp4_send     = false;
    ulong resp4_time    = 0; 
    ulong resp4_delay   = 25;   


class router
{
private:
    ringbuffer      *Wsbuffer;
    Ws2900Data      *WsData;
    SoftwareSerial  *WsSerial;
    NTPClient       *NtpClient;
    
public:
    /// @brief init the router class
    /// @param buffer 
    /// @param data 
    /// @param serial 
    void begin(ringbuffer *buffer, Ws2900Data *data, SoftwareSerial *serial, NTPClient *ntpClient);
    
    /// @brief has to execute in main loop to process data
    void route();

    /// @brief add checksum at last position of array
    /// @param array 
    void checkSum_8Mod256(char *array);

    /// @brief create array of date in pattern of WS2900;
    /// @param array 
    void createDate(char *array);
};


#endif