#include "router.h"


bool resp1_send = false;
ulong resp1_time = 0;
ulong resp1_delay = 10;     //10ms


void router::begin(ringbuffer *buffer, Ws2900Data *data)
{
    Wsbuffer = buffer;
    WsData = data;
}

void router::route()
{
    //check for new data and route
    while(Wsbuffer->available())
    {
        char *temp = Wsbuffer->getSegment(4);

        if(strncmp(temp, patt1, 4) == 0)
        {
            //indicate that we ned to send response
            resp1_send = true;
            //save time
            resp1_time = millis();
        }
        //read weather data from received data
        else if(strncmp(temp, patt2, 4) == 0)
        {
            WsData->set_newData(&Wsbuffer->getSegment(Wsbuffer->bufferTail, 100));
        }
        else if(strncmp(temp, patt1, 4) == 0)
        {

        }
        else if(strncmp(temp, patt1, 4) == 0)
        {

        }
    }

    //send responses
    if(resp1_send && millis() - resp1_time > resp1_delay)
    {
        //send resp1
    }


}