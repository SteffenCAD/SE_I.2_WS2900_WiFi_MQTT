#ifndef _ROUTER_H_
#define _ROUTER_H_

#include "Arduino.h"
#include "ringbuffer.h"
#include "Ws2900Data.h"
#include "SoftwareSerial.h"

#define bufSize 300

#define printRawData



class router
{

private:
    ringbuffer *Wsbuffer;
    Ws2900Data *WsData;
    SoftwareSerial *WsSerial;

public:
    void begin(ringbuffer *buffer, Ws2900Data *data, SoftwareSerial *serial);
    void route();

};


#endif