#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

#define MESSAGE_SIZE    18

/* Debug definitions, comment in and out as needed */
//#define DEBUG_COMMS
//#define DEBUG_BITRATE


namespace Comms {
    extern volatile bool sending, received, data;
    extern volatile bool enable;
}

uint16_t checksum16(uint8_t *data, uint16_t len);

#endif // MAIN_H
