#ifndef _ROUTER_H_
#define _ROUTER_H_

#include "Arduino.h"
#include "ringbuffer.h"
#include "Ws2900Data.h"

#define bufSize 300


//define patterns
char patt1[] = {0x00, 0x07, 0x3b, 0x44};
char resp1[] = {0x0b, 0xFF};

char patt2[] = {0x00, 0x07, 0x3b, 0x45};
char resp2[] = {0x0b, 0xFF};

class router
{
public:
    void begin(ringbuffer* buffer, Ws2900Data* data);
    void route();

private:
    ringbuffer *Wsbuffer;
    Ws2900Data *WsData;
};


#endif _ROUTER_H_