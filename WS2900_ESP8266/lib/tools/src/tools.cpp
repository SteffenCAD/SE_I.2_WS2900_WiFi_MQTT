#include "tools.h"


/// @brief calculate checksum of array
/// @param array 
/// @return 
uint8_t CheckSum_8Mod256(char *array)
{
    uint32_t sum = 0;
    uint16_t size = sizeof(array);
    uint8_t checksum = 0;

    //
    for(uint16_t i = 0; i< size; i++)
    {
        sum += array[i];
    }

    checksum = sum % 256;

    return checksum;
}