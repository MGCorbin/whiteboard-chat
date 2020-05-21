#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <QColor>
#include <QLine>
#include <QMetaClassInfo>

#include <pthread.h>
#include <stdint.h>

typedef union
{
    int16_t VAL;
    struct
    {
        int8_t BYTE0;
        int8_t BYTE1;
    };
} union16_t;


typedef union
{
    uint8_t vals[10];               // all our messages are 10 bytes long...
    struct
    {
        union                       // there are then options for how we read and write our messages
        {
            char colour[8];         // can be an array of chars that reperesent a colour
            int16_t data[4];        // or can be an array of numbers that can be uses to send coordiantes etc.
        };                          // as these live at the same address in memory, we can write the data in a general way and read it back in a speicifc way
        uint16_t checksum;          // all our messages have a checksum
    };
} comms_message_t;                  // this makes encoding and decoding our messages easier


#endif // TYPEDEFS_H
