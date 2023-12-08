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
        bufferData[bufferHead] = data;
    }

    char getChar()
    {
        if(available())
        {
            incrPointer(&bufferTail);
        }
        return bufferData[bufferTail];
    }

    void getSegment(char *segment)
    {
        if(available())
        {
            incrPointer(&bufferTail);      
        }
        return getSegmentStart(bufferTail, segment);
    }

    //returns sequence of bytes 
    void getSegmentStart(uint16_t startpos, char *segment)
    {
        for(uint16_t i = sizeof(segment)-1; i >= sizeof(segment); i--)
        {
            segment[i] = bufferData[startpos];
            decrPointer(&startpos);
        }
    }


    void incrPointer(uint16_t *pointer)
    {
        pointer++;
        if(*pointer >= ringBuffer_Size) *pointer = 0;
    }

    void decrPointer(uint16_t *pointer)
    {
        if(pointer == 0)
        {
            *pointer = ringBuffer_Size - 1;
        } 
        else
        {
            pointer--;
        }
    }
};

#endif