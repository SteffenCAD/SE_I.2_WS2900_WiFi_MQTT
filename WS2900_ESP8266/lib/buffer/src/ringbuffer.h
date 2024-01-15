#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include"Arduino.h"

#define ringBuffer_Size 300 //only uint16_t is allowed


class ringbuffer
{
private:
    bool        doNextSegment = false;  //enables next segment
    bool        nextSegmentState =false;    //if true, next segment is complete
    uint16_t    nextSegmentSize = 0;    //number of bytes of next segment
    uint16_t    nextSegmentCount = 0;   //current counter of next segment
    HardwareSerial  *_serial;

public:
    char bufferData[ringBuffer_Size];

    uint16_t bufferHead = 0;
    uint16_t bufferTail = 0;

    void begin(HardwareSerial *serial)
    {
        _serial = serial;
    }

    void loop()
    {
        //read all bytes to ring buffer
        while(_serial->available() > 0 )
        {
            char input = _serial->read();
            this->add(input);
        }
    }

    bool available()
    {
        if(bufferTail != bufferHead)
        {
            return true;
        }
        return false;
    }

    void add(char data)
    {
        incrPointer(&bufferHead);
        strncpy(&bufferData[bufferHead], &data, 1);
    }

    char getChar()
    {
        if(available())
        {
            incrTail();
        }
        char temp;
        strncpy(&temp, &bufferData[bufferTail], 1);
        return temp;
    }

    void getSegment(char *segment, uint16_t size)
    {
        if(available())
        {     
            incrTail();
        }
        getSegmentStart(bufferTail, segment, size);
    }

    void getSegmentStart(uint16_t startpos, char *segment, uint16_t size)
    {

        for(uint16_t i = size; i > 0; i--)
        {
            segment[i-1] = bufferData[startpos];
            decrPointer(&startpos);
            //segment[i-1] = bufferData[startpos-(size-i)];
        }
    }

    void getNextSegment(uint16_t size)
    {
        //check if data is allready available
        if(diffTailHead() >= size)
        {
            nextSegmentState = true;
        }
        else
        {
            //save size
            nextSegmentSize = size;
            //enable next segment
            doNextSegment = true;
        }
    }

    bool nextSegmentAvailable()
    {
        bool retval = nextSegmentState;
        //if new data is true, reset
        if(nextSegmentState) nextSegmentState = false;
        //return state of available data
        return retval;     
    }

    void incrTail()
    {
        incrPointer(&bufferTail);

        //next Segment
        if(doNextSegment)
        {
            nextSegmentCount++;
            if(nextSegmentCount >= nextSegmentSize)
            {
                //reset mechanics
                doNextSegment       = false;
                nextSegmentCount    = 0;

                //indicate available data
                nextSegmentState    = true;
            }
        }
    }

    /// @brief return difference between tail and head of the buffer
    /// @return 
    uint16_t diffTailHead()
    {
        uint16_t counter = 0;
        uint16_t tempTail = bufferTail;

        while(tempTail != bufferHead)
        {
            incrPointer(&tempTail);
            counter++;
        }

        return counter;
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