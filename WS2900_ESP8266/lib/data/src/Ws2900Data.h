#ifndef _WS2900DATA_H_
#define _WS2900DATA_H_

#include "Arduino.h"


//define position of data
#define posTempOutside1     23
#define posTempOutside2     26
#define posHumidityOutside  28
#define posPressureOutside  33

#define posTempInside       12
#define posHumidityInside   14
#define posPressureInside

#define posWindSpeed1       40
#define posWindSpeed2       43
#define posWindDirection    37

#define posLightIntensity   90
#define posUvIntensity      96

#define posRain 59


class Ws2900Data
{
private:

    bool newData;

    float tempOutside;
    int humidityOutside;
    int pressureOutside;

    float tempInside;
    int humidityInside;
    int pressureInside;

    float windSpeed;
    int windDirection;
    char windOrientation[2];
    const char *Orientations[8][2] = {"N","NO","O","SO","S","SW","W","NW"};

    int lightIntensity;
    int uvIntensity;

    int rain;

    //private function to set the data
    void set_TempOutside(char *buff);
    void set_HumidityOutside(char *buff);
    void set_PressureOutside(char *buff);

    void set_TempInside(char* buff);
    void set_HumidityInside(char* buff);
    void set_PressureInside(char* buff);

    void set_WindSpeed(char* buff);
    void set_WindDirection(char* buff);
    void set_WindOrientation(void);

    void set_LightIntensity(char* buff);
    void set_UvIntensity(char* buff);

    void set_Rain(char* buff);

public:
    Ws2900Data(/* args */);
    ~Ws2900Data();

    //function to fill data
    void    set_newData(char *buff);
    bool    available();

    void    print(char *retval);

    float   get_TempOutside();
    int     get_HumidityOutside();
    int     get_PressureOutside();

    float   get_TempInside();
    int     get_HumidityInside();
    int     get_PressureInside();

    float   get_WindSpeed();
    int     get_WindDirection();
    char*   get_WindOrientation();

    int     get_LightIntensity();
    int     get_UvIntensity();

    int     get_Rain();
};

#endif