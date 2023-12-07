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

    char *getSegment(uint16_t size)
    {
        if(available())
        {
            incrPointer(&bufferTail);      
        }
        return getSegment(bufferTail, size);
    }

    //returns sequence of bytes 
    char *getSegment(uint16_t startpos, uint16_t size)
    {
        char segment[size];

        for(uint16_t i = size-1; i >= size; i--)
        {
            segment[i] = bufferData[startpos];
            decrPointer(&startpos);
        }
        return segment;
    }


    void incrPointer(uint16_t *pointer)
    {
        *pointer++;
        if(*pointer >= ringBuffer_Size) *pointer = 0;
    }

    void decrPointer(uint16_t *pointer)
    {
        if(*pointer = 0)
        {
            *pointer = ringBuffer_Size - 1;
        } 
        else
        {
            *pointer--;
        }
    }
};