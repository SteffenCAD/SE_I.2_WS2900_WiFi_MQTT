#ifndef _TIMESYNC_H_
#define _TIMESYNC_H_

#include "Arduino.h"


class timesync
{
    private:
        uint8_t year = 23;
        uint8_t month = 12;
        uint8_t day = 12;

        uint8_t hour = 9;
        uint8_t minute = 30;
        uint8_t second = 30;

    public:
        uint8_t get_hour();
        uint8_t get_minute();
        uint8_t get_second();

        uint8_t get_year();
        uint8_t get_month();
        uint8_t get_day();
};


#endif