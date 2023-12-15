#ifndef _WS2900DATA_H_
#define _WS2900DATA_H_

#include "Arduino.h"


//define position of data
#define posTempOutside      16
#define posHumidityOutside  28
#define posPressureOutside  33

#define posTempInside       11
#define posHumidityInside   14

#define posWindSpeed1       40
#define posWindSpeed2       43
#define posWindDirection    36

#define posLightIntensity   90
#define posUvIntensity      96

#define posRain 55


class Ws2900Data
{
private:

    bool newData;

    float tempOutside;
    uint8_t humidityOutside;
    float pressureOutside;

    float tempInside;
    uint8_t humidityInside;
    float pressureInside;

    float windSpeed;
    uint16_t windDirection;
    char windOrientation[2];
    const char *Orientations[8][2] = {"N","NO","O","SO","S","SW","W","NW"};

    float lightIntensity;
    uint8_t uvIntensity;

    float rain;             //mm/h

    //private function to set the data
    void set_TempOutside(char *buff);
    void set_HumidityOutside(char *buff);
    void set_PressureOutside(char *buff);

    void set_TempInside(char* buff);
    void set_HumidityInside(char* buff);

    void set_WindSpeed(char* buff);
    void set_WindDirection(char* buff);
    void set_WindOrientation(void);

    void set_LightIntensity(char* buff);
    void set_UvIntensity(char* buff);

    void set_Rain(char* buff);

public:
    Ws2900Data(/* args */);
    ~Ws2900Data();

    void get_time(char buff);

    //function to fill data
    void    set_newData(char *buff);
    bool    available();

    void    print(char *retval);

    float   get_TempOutside();
    uint8_t get_HumidityOutside();
    float   get_PressureOutside();

    float   get_TempInside();
    uint8_t get_HumidityInside();

    float   get_WindSpeed();
    uint16_t get_WindDirection();
    char*   get_WindOrientation();

    float   get_LightIntensity();
    uint8_t get_UvIntensity();

    float get_Rain();
};

#endif