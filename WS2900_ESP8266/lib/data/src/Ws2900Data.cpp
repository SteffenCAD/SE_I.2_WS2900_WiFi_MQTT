#include "Ws2900Data.h"

Ws2900Data::Ws2900Data(/* args */)
{
    //initialize values to a default
    newData = false;

    tempOutside     = 0.00;
    humidityOutside = 0;
    pressureOutside = 0;

    tempInside      = 0.00;
    humidityInside  = 0;

    windSpeed       = 0.00;
    windDirection   = 0;
    
    lightIntensity = 0.00;
    uvIntensity = 0;

    rain = 0;

    windOrientation[0] = 'N';
}



#pragma region private_functions

    void Ws2900Data::set_time(char *buff)
    {
        year    = buff[posTimeYear];
        month   = buff[posTimeMonth];
        day     = buff[posTimeDay];
        hour    = buff[posTimeHour];
        minute  = buff[posTimeMin];
        second  = buff[posTimeSec];
    }

    void Ws2900Data::set_TempOutside(char *buff)
    {
        tempOutside = (float)((buff[posTempOutside]*256 +buff[posTempOutside + 1])/10.00  -40.0 );
    }
    
    void Ws2900Data::set_HumidityOutside(char* buff)
    {
        humidityOutside = buff[posHumidityOutside];
    }
    
    void Ws2900Data::set_PressureOutside(char* buff)
    {
        pressureOutside = ((buff[posPressureOutside] << 8) | (buff[posPressureOutside + 1]))/10.00;
    }

    void Ws2900Data::set_TempInside(char* buff)
    {
        tempInside = (float)((buff[posTempInside]*256 +buff[posTempInside + 1])/10.00  -40.0 );
    }
    
    void Ws2900Data::set_HumidityInside(char* buff)
    {
        humidityInside = buff[posHumidityInside];
    }

    void Ws2900Data::set_WindSpeed(char* buff)
    {
        windSpeed = (float)buff[posWindSpeed1] / 2.8;
    }
    
    void Ws2900Data::set_WindDirection(char* buff)
    {
        windDirection = (buff[posWindDirection] << 8) | (buff[posWindDirection+1]);
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
        lightIntensity = ((buff[posLightIntensity] << 8) | buff[posLightIntensity]) / 10000.00;
    }
    
    void Ws2900Data::set_UvIntensity(char* buff)
    {
        uvIntensity = buff[posUvIntensity];
    }

    void Ws2900Data::set_Rain(char* buff)
    {
        rain = ((buff[posRain] << 8) | buff[posRain + 1])/10.0;
    }
#pragma endregion


#pragma region public_functions

void Ws2900Data::set_newData(char *buff)
{
    set_time(buff);
    set_TempOutside(buff);
    set_HumidityOutside(buff);
    set_PressureOutside(buff);
    set_TempInside(buff);
    set_HumidityInside(buff);
    set_WindSpeed(buff);
    set_WindDirection(buff);
    //set_WindOrientation();
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

String Ws2900Data::toString()
{
    String retval = "";

    retval += "TempOut: "   + String(tempOutside,2)     + " degC |";
    retval += "HumiOut: "   + String(humidityOutside)   + " % | ";
    retval += "PresOut: "   + String(pressureOutside,2) + " mBar | ";
    retval += "TemIn: "     + String(tempInside,2)      + " degC | ";
    retval += "HumiIn "     + String(humidityInside)    + " % | ";
    retval += "WindSp: "    + String(windSpeed,2)       + " m/s | ";
    retval += "WindDr: "    + String(windDirection)     + " deg | ";
    retval += "LighIn: "    + String(lightIntensity,2)  + " klux | ";
    retval += "UvIn: "      + String(uvIntensity)       + " | ";
    retval += "Rain: "      + String(rain,2)            + " mm/h";

    return retval;
}


String Ws2900Data::toJson(bool debug)
{
    String result = "{\n";
    // append debug
    result += "  \"debug\": " + String(debug ? "true" : "false") + ",\n";
    // append sensors
    result += "  \"sensors\": [\n";

    //add sensor
    result += "    {\n";
    result += "      \"sensorid\": 1,\n";
    result += "      \"value\": " +  String(tempOutside,2) + ",\n";
    result += "      \"timestamp\": \"" + get_time() + "\"\n";
    result += "    },\n";
        
    //add sensor
    result += "    {\n";
    result += "      \"sensorid\": 2,\n";
    result += "      \"value\": " +  String(humidityOutside) + ",\n";
    result += "      \"timestamp\": \"" + get_time() + "\"\n";
    result += "    },\n";
    //add sensor
    result += "    {\n";
    result += "      \"sensorid\": 3,\n";
    result += "      \"value\": " +  String(pressureOutside,2) + ",\n";
    result += "      \"timestamp\": \"" + get_time() + "\"\n";
    result += "    },\n";
    //add sensor
    result += "    {\n";
    result += "      \"sensorid\": 4,\n";
    result += "      \"value\": " +  String(windSpeed,2) + ",\n";
    result += "      \"timestamp\": \"" + get_time() + "\"\n";
    result += "    },\n";
    //add sensor
    result += "    {\n";
    result += "      \"sensorid\": 5,\n";
    result += "      \"value\": " +  String(windDirection) + ",\n";
    result += "      \"timestamp\": \"" + get_time() + "\"\n";
    result += "    },\n";
    //add sensor
    result += "    {\n";
    result += "      \"sensorid\": 6,\n";
    result += "      \"value\": " +  String(lightIntensity,2) + ",\n";
    result += "      \"timestamp\": \"" + get_time() + "\"\n";
    result += "    },\n";
    //add sensor
    result += "    {\n";
    result += "      \"sensorid\": 7,\n";
    result += "      \"value\": " +  String(uvIntensity) + ",\n";
    result += "      \"timestamp\": \"" + get_time() + "\"\n";
    result += "    },\n";
    //add sensor
    result += "    {\n";
    result += "      \"sensorid\": 8,\n";
    result += "      \"value\": " +  String(rain,2) + ",\n";
    result += "      \"timestamp\": \"" + get_time() + "\"\n";
    result += "    }\n";

    result += " ]\n} ";

    return result;
}

String Ws2900Data::get_time()
{
    return String((uint16_t)year + 2000) + "-" + String(month)+ "-" + String(day)+ "T" 
            + String(hour) + ":" + String(minute) + ":" + String(second) + ".000Z";
}

float   Ws2900Data::get_TempOutside()       {return tempOutside;}

uint8_t Ws2900Data::get_HumidityOutside()   {return humidityOutside;}

float   Ws2900Data::get_PressureOutside()   {return pressureOutside;}

float   Ws2900Data::get_TempInside()        {return tempInside;}

uint8_t Ws2900Data::get_HumidityInside()    {return humidityInside;}

float   Ws2900Data::get_WindSpeed()         {return windSpeed;}

uint16_t Ws2900Data::get_WindDirection()     {return windDirection;}

char*   Ws2900Data::get_WindOrientation()   {return windOrientation;}

float Ws2900Data::get_LightIntensity()    {return lightIntensity;}

uint8_t Ws2900Data::get_UvIntensity()       {return uvIntensity;}

float Ws2900Data::get_Rain()              {return rain;}


#pragma endregion