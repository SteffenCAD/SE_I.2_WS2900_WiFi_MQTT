#include "router.h"


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
byte resp2[]        = {0x22,0x00,0x2E,0x00,0x00,0x0E,0x10,0x00,0xFF,0x07,0x38,0x10,0x15,0x45,0x75,0x72,0x6F,0x70,0x65,0x2F,0x42,0x65,0x72,0x6C,0x69,0x6E,
                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCC};
bool resp2_send     = false;
ulong resp2_time    = 0;    //save time when response was received
ulong resp2_delay   = 9;    //10ms

//pattern3  ################################################################################################################################################
//time response 2
char patt3[]        = {0x06, 0x00, 0x04, 0x0A};
byte resp3[]        = {0x06,0x00,0x0B,0x17,0x0C,0x09,0x14,0x38,0x2E,0x00,0xB7};
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
byte resp5[]        = {0x20,0x00,0x0A,0x34,0x94,0x54,0x96,0x1F,0x52,0x4D};

bool resp5_send     = false;
ulong resp5_time    = 0;    //save time when response was received
ulong resp5_delay   = 9;    //20ms

//pattern4  ################################################################################################################################################
// weather data
//##########################################################################################################################################################
//char patt4[]        = {0x4C,0x09,0x00,0x00,0x00,0x00,0x7A,0x04,0x00};
char patt4[]        = {0x4C,0x09,0x00,0x00};
byte resp4[]        = {0x01,0x00,0x07,0x01,0x04,0x04,0x11,0x01,0x00,0x09,0x01,0x04,0x04,0x04,0x04,0x1B};

bool resp4_send     = false;
ulong resp4_time    = 0; 
ulong resp4_delay   = 25;   


void router::begin(ringbuffer *buffer, Ws2900Data *data, SoftwareSerial *serial)
{
    //save reference to object
    Wsbuffer    = buffer;
    WsData      = data;
    WsSerial    = serial;
}

void router::route()
{
    //check for new data and route
    while(Wsbuffer->available())
    {
        Wsbuffer->getSegment(temp1, sizeof(temp1));


        if(strncmp(temp1, patt1, 4) == 0)
        {       
            //indicate that we ned to send response
            resp1_send = true;
            resp1_time = millis();
        }
        //read weather data from received data
        else if(strncmp(temp1, patt2, 4) == 0)
        {
            resp2_send = true;
            resp2_time = millis();
        }
        else if(strncmp(temp1, patt3, 4) == 0)
        {   
            resp3_send = true;
            resp3_time = millis();
        }
        else if(strncmp(temp1, patt5, 4) == 0)
        {   
            resp5_send = true;
            resp5_time = millis();
        }        
        else if(strncmp(temp1, patt4, 4) == 0)
        {
            resp4_send = true;
            resp4_time = millis();

            //get raw data from ring buffer (size of WsRawData array), start pos = current read
            Wsbuffer->getSegment(WsRawData, sizeof(WsRawData));

            #ifdef printRawData
            for(uint16_t i =0; i< sizeof(WsRawData); i++)
            {
                Serial.printf("%2x;",WsRawData[i]);
            }
            Serial.println();
            #endif

            WsData->set_newData(WsRawData);
        }
    }


    //send responses after response delay
    if(resp1_send && millis() > (resp1_time + resp1_delay))
    {
        //reset
        resp1_send = false;
        resp1_time = 0;

        //send response
        for(int i = 0; i < sizeof(resp1); i++)
        {
            WsSerial->write(resp1[i]);
        }
    }
    else if(resp2_send && millis() > (resp2_time + resp2_delay))
    {
        //reset
        resp2_send = false;
        resp2_time = 0;

        //send response
        for(int i = 0; i < sizeof(resp2); i++)
        {
            WsSerial->write(resp2[i]);
        }
    }
    else if(resp3_send && millis() > (resp3_time + resp3_delay))
    {
        //reset
        resp3_send = false;
        resp3_time = 0;

        //send response
        for(int i = 0; i < sizeof(resp3); i++)
        {
            WsSerial->write(resp3[i]);
        }
    }
    else if(resp4_send && millis() > (resp4_time + resp4_delay))
    {
        //reset
        resp4_send = false;
        resp4_time = 0;

        //send response
        for(int i = 0; i < sizeof(resp4); i++)
        {
            WsSerial->write(resp4[i]);
        }
    }
    else if(resp5_send && millis() > (resp5_time + resp5_delay))
    {
        //reset
        resp5_send = false;
        resp5_time = 0;

        //send response
        for(int i = 0; i < sizeof(resp5); i++)
        {
            WsSerial->write(resp5[i]);
        }
    }
}