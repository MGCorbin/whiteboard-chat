#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <pthread.h>
#include <stdint.h>

//#define DEBUG_COMMS

typedef struct
{
    volatile bool sending;
    volatile bool receiving;
    volatile bool data;
    pthread_mutex_t comms_mutex;
} comms_signals_t;

typedef union
{
    uint16_t VAL;
    struct
    {
        uint8_t BYTE0;
        uint8_t BYTE1;
    };
} union16_t;

#endif // TYPEDEFS_H
