#ifndef _WS2900DATA_H_
#define _WS2900DATA_H_

#include "Arduino.h"
#include <string>
#include <iostream>
#include <sstream>

//define position of data

#define posTimeYear     4
#define posTimeMonth    5
#define posTimeDay      6
#define posTimeHour     7
#define posTimeMin      8
#define posTimeSec      9

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


#define mqttNumOfValuesToSend 8;

class Ws2900Data
{
private:

    //time data
    uint8_t year;
    uint8_t month; 
    uint8_t day;
    uint8_t hour; 
    uint8_t minute;
    uint8_t second; 

    //sensor data
    float tempOutside;
    uint8_t humidityOutside;
    float pressureOutside;

    float tempInside;
    uint8_t humidityInside;

    float windSpeed;
    uint16_t windDirection;


    float lightIntensity;
    uint8_t uvIntensity;

    float rain;             //mm/h


    bool newData;
    char windOrientation[2];
    const char *Orientations[8][2] = {"N","NO","O","SO","S","SW","W","NW"};

    //private function to set the data
    void set_time(char *buff);

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

    void get_time(char buff);
    String get_time();
    //function to fill data
    void    set_newData(char *buff);
    bool    available();

    String  toString();
    String  toJson(bool debug);


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