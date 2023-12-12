#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include"Arduino.h"

#define ringBuffer_Size 300 //only uint16_t is allowed


class ringbuffer
{
private:


public:

    char bufferData[ringBuffer_Size];

    uint16_t bufferHead = 0;
    uint16_t bufferTail = 0;

    bool available()
    {
        if(bufferTail != bufferHead)
        {
            return true;
        }
        return false;
    }

    //add 
    void add(char data)
    {
        incrPointer(&bufferHead);
        strncpy(&bufferData[bufferHead], &data, 1);
    }

    char getChar()
    {
        if(available())
        {
            incrPointer(&bufferTail);
        }
        char temp;
        strncpy(&temp, &bufferData[bufferTail], 1);
        return temp;
    }

    void getSegment(char *segment, uint16_t size)
    {
        if(available())
        {
            incrPointer(&bufferTail);      
        }

        getSegmentStart(bufferTail, segment, size);
    }

    void getSegmentStart(uint16_t startpos, char *segment, uint16_t size)
    {
        for(uint16_t i = size; i > 0; i--)
        {
            segment[i-1] = bufferData[startpos-(size-i)];
        }
    }


    void incrPointer(uint16_t *pointer)
    {
        int tempPointer = *pointer;
        tempPointer++;

        if(tempPointer >= ringBuffer_Size) tempPointer = 0;

        *pointer = tempPointer;
    }

    void decrPointer(uint16_t *pointer)
    {
        int tempPointer = *pointer;
        if(tempPointer == 0)
        {
            tempPointer = ringBuffer_Size - 1;
        } 
        else
        {
            tempPointer--;
        }
        *pointer = tempPointer;
    }
};

#endif