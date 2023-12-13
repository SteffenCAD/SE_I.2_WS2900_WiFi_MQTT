#include "router.h"

void router::begin(ringbuffer *buffer, Ws2900Data *data, SoftwareSerial *serial, NTPClient *ntpClient)
{
    //save reference to object
    Wsbuffer    = buffer;
    WsData      = data;
    WsSerial    = serial;
    NtpClient   = ntpClient;
    
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
            //enable get next segment from buffer
            Wsbuffer->getNextSegment(sizeof(WsRawData) - sizeof(patt4));
        }

        //if next segment is available
        if(Wsbuffer->nextSegmentAvailable())
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

        //get current time and calculate chesum
        createDate(resp3);
        checkSum_8Mod256(resp3);

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

void router::createDate(char *array)
{
    struct tm *ptm = gmtime ((time_t *)NtpClient->getEpochTime());
    array[0] = 0x06;
    array[1] = 0x00;
    array[2] = 0x0B;
    array[3] = (char)ptm->tm_year-2000;
    array[4] = (char)ptm->tm_mon;
    array[5] = (char)ptm->tm_mday;
    array[6] = (char)ptm->tm_hour;
    array[7] = (char)ptm->tm_min;
    array[8] = (char)ptm->tm_sec;
    array[9] = 0x00;
    array[10] = 0x00;
}

void router::checkSum_8Mod256(char *array)
{
    uint32_t sum = 0;
    uint16_t size = sizeof(array);
    uint8_t checksum = 0;

    //
    for(uint16_t i = 0; i< size-1; i++)
    {
        sum += array[i];
    }

    array[size-1] = sum % 256;
}