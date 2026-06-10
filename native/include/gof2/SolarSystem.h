#ifndef GOF2_SOLARSYSTEM_H
#define GOF2_SOLARSYSTEM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct SolarSystem {
    int field_0x0;                      // +0x0
    int field_0x4;                      // +0x4
    int field_0x8;                      // +0x8
    int field_0x18;                     // +0x18
    int field_0x1c;                     // +0x1c
    uint32_t field_0x20;                // +0x20
    int field_0x24;                     // +0x24
    int field_0x28;                     // +0x28
    int field_0x2c;                     // +0x2c
    int field_0x30;                     // +0x30
    int field_0x34;                     // +0x34
    uint32_t* field_0x38;               // +0x38
    void* field_0x3c;                   // +0x3c
    uint32_t* field_0x40;               // +0x40
};
#endif
