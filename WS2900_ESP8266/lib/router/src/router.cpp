#include "router.h"


char temp1[4]; 

//define patterns
//pattern1
char patt1[]        = {0x00, 0x07, 0x3b, 0x44};
char resp1[]        = {0x0b, 0xFF};
bool resp1_send     = false;
ulong resp1_time    = 0;
ulong resp1_delay   = 10;     //10ms

//pattern2
char patt2[]        = {0x00, 0x07, 0x3b, 0x45};
char resp2[]        = {0x0b, 0xFF};
bool resp2_send     = false;
ulong resp2_time    = 0;
ulong resp2_delay   = 10;     //10ms


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
        Wsbuffer->getSegment(temp1);

        if(strncmp(temp1, patt1, 4) == 0)
        {
            //indicate that we ned to send response
            resp1_send = true;
            //save time
            resp1_time = millis();
        }
        //read weather data from received data
        else if(strncmp(temp1, patt2, 4) == 0)
        {
            char temp2[100];
            Wsbuffer->getSegmentStart(Wsbuffer->bufferTail, temp2);
            WsData->set_newData(temp2);
        }
        else if(strncmp(temp1, patt1, 4) == 0)
        {

        }
        else if(strncmp(temp1, patt1, 4) == 0)
        {

        }
    }

    //send responses after response delay
    if(resp1_send && millis() - resp1_time > resp1_delay)
    {
        //reset
        resp1_send = false;
        resp1_time = 0;

        //send response
        WsSerial->write(resp1);
    }
}