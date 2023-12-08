#include "Ws2900Data.h"

Ws2900Data::Ws2900Data(/* args */)
{
    //initialize values to a default
    newData = false;

    tempOutside = 0.00;
    humidityOutside = 0;
    pressureOutside = 0;

    tempInside = 0.00;
    humidityInside = 0;
    pressureInside = 0;

    windSpeed = 0.00;
    windDirection = 0;
    windOrientation[0] = 'N';

    lightIntensity = 0.00;
    uvIntensity = 0;

    rain = 0;
}

Ws2900Data::~Ws2900Data()
{
}


#pragma region private_functions
    void Ws2900Data::set_TempOutside(char *buff)
    {
        tempOutside = (float)(buff[posTempOutside1] * 4.50);
    }
    
    void Ws2900Data::set_HumidityOutside(char* buff)
    {
        humidityOutside = buff[posHumidityOutside];
    }
    
    void Ws2900Data::set_PressureOutside(char* buff)
    {
        pressureOutside = ((int)buff[posPressureOutside]) | buff[posPressureOutside + 1];
    }

    void Ws2900Data::set_TempInside(char* buff)
    {
        tempInside = (float)(buff[posTempInside] * 4.50);
    }
    
    void Ws2900Data::set_HumidityInside(char* buff)
    {
        humidityInside = buff[posHumidityInside];
    }
    
    void Ws2900Data::set_PressureInside(char* buff)
    {
        pressureInside = ((int)buff[posPressureOutside]) | buff[posPressureOutside + 1];
    }

    void Ws2900Data::set_WindSpeed(char* buff)
    {
        windSpeed = (float)buff[posWindSpeed1] * 1.00;
    }
    
    void Ws2900Data::set_WindDirection(char* buff)
    {
        windDirection = buff[posWindDirection];
    }
    
    void Ws2900Data::set_WindOrientation()
    {
        //convert wind direction to wind orientation as string
        if(windDirection >= 338 && windDirection <= 22){strcpy(windOrientation,*Orientations[0]);}
        if(windDirection >= 23  && windDirection <= 67){strcpy(windOrientation,*Orientations[1]);}
        if(windDirection >= 68  && windDirection <= 112){strcpy(windOrientation,*Orientations[2]);}
        if(windDirection >= 113 && windDirection <= 157){strcpy(windOrientation,*Orientations[3]);}
        if(windDirection >= 158 && windDirection <= 202){strcpy(windOrientation,*Orientations[4]);}
        if(windDirection >= 203 && windDirection <= 247){strcpy(windOrientation,*Orientations[5]);}
        if(windDirection >= 248 && windDirection <= 292){strcpy(windOrientation,*Orientations[6]);}
        if(windDirection >= 293 && windDirection <= 337){strcpy(windOrientation,*Orientations[7]);}
    }

    void Ws2900Data::set_LightIntensity(char* buff)
    {
        lightIntensity = buff[posLightIntensity];
    }
    
    void Ws2900Data::set_UvIntensity(char* buff)
    {
        uvIntensity = buff[posUvIntensity];
    }

    void Ws2900Data::set_Rain(char* buff)
    {
        rain = buff[posRain];
    }
#pragma endregion


#pragma region public_functions

void Ws2900Data::set_newData(char *buff)
{
    set_TempOutside(buff);
    set_HumidityOutside(buff);
    set_PressureOutside(buff);

    set_TempInside(buff);
    set_HumidityInside(buff);
    set_PressureInside(buff);

    set_WindSpeed(buff);
    set_WindDirection(buff);
    set_WindOrientation();
    set_LightIntensity(buff);
    set_UvIntensity(buff);

    set_Rain(buff);

    //inicate that new Data is available
    newData = true;
}

bool Ws2900Data::available()
{
    bool retval = newData;
    //if new data is true, reset
    if(newData) newData = false;
    //return state of available data
    return retval;
}

void Ws2900Data::print(char *retval)
{
    //char retval[200];

    sprintf(retval, "TempOut: %f | HumiOut: %d | PresOut: %d | TemIn: %f | HumiIn %d | PresIn %d | WindSp: %f | WindDr: %d | LighIn: %d | UvIn: %d | Rain: %d",
                    tempOutside, humidityOutside, pressureOutside, tempInside, humidityInside, pressureInside, windSpeed, windDirection, lightIntensity, uvIntensity, rain);

    //return retval;
}

float   Ws2900Data::get_TempOutside()       {return tempOutside;}

int     Ws2900Data::get_HumidityOutside()   {return humidityOutside;}

int     Ws2900Data::get_PressureOutside()   {return pressureOutside;}

float   Ws2900Data::get_TempInside()        {return tempInside;}

int     Ws2900Data::get_HumidityInside()    {return humidityInside;}

int     Ws2900Data::get_PressureInside()    {return pressureInside;}

float   Ws2900Data::get_WindSpeed()         {return windSpeed;}

int     Ws2900Data::get_WindDirection()     {return windDirection;}

char*   Ws2900Data::get_WindOrientation()   {return windOrientation;}

int     Ws2900Data::get_LightIntensity()    {return lightIntensity;}

int     Ws2900Data::get_UvIntensity()       {return uvIntensity;}

int     Ws2900Data::get_Rain()              {return rain;}


#pragma endregion