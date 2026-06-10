#ifndef GOF2_STATION_H
#define GOF2_STATION_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Station {
    int field_0xc;                      // +0xc
    int field_0x10;                     // +0x10
    int field_0x18;                     // +0x18
    uint8_t field_0x1c;                 // +0x1c
    int field_0x20;                     // +0x20
    uint8_t field_0x24;                 // +0x24
    uint32_t* field_0x28;               // +0x28
    void* field_0x2c;                   // +0x2c
    void* field_0x30;                   // +0x30
    uint8_t field_0x71;                 // +0x71
};
#endif
