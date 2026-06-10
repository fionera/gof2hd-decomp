#ifndef GOF2_RADIO_H
#define GOF2_RADIO_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Radio {
    void* field_0x0;                    // +0x0
    RadioMessage* field_0x4;            // +0x4
    void* field_0x8;                    // +0x8
    void* field_0xc;                    // +0xc
    void* field_0x10;                   // +0x10
    int64_t field_0x18;                 // +0x18
    int field_0x28;                     // +0x28
    uint8_t field_0x2c;                 // +0x2c
    uint8_t field_0x2d;                 // +0x2d
    int field_0x30;                     // +0x30
    String* field_0x34;                 // +0x34
    int field_0x38;                     // +0x38
    int field_0x3c;                     // +0x3c
    int field_0x40;                     // +0x40
};
#endif
