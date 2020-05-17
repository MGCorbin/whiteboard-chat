#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <QColor>
#include <QLine>
#include <QMetaClassInfo>

#include <pthread.h>
#include <stdint.h>

//#define DEBUG_COMMS

#define PACKET_SIZE     17

//typedef struct
//{
//    volatile bool sending;
//    volatile bool receiving;
//    volatile bool data;
//    pthread_mutex_t comms_mutex;
//} comms_signals_t;

namespace Comms {
    extern volatile bool send_receive, data;
}


typedef union
{
    int16_t VAL;
    struct
    {
        int8_t BYTE0;
        int8_t BYTE1;
    };
} union16_t;

typedef struct
{
    QLine line;
    QColor col;
} draw_data_t;

Q_DECLARE_METATYPE(draw_data_t);

#endif // TYPEDEFS_H
