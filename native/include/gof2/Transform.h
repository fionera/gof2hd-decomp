#ifndef GOF2_TRANSFORM_H
#define GOF2_TRANSFORM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Transform {
    uint field_0x3c;                    // +0x3c
    char** field_0x40;                  // +0x40
    uint field_0x4c;                    // +0x4c
    Transform** field_0x50;             // +0x50
    AEMath::Vector field_0xd4;          // +0xd4
    float field_0xe0;                   // +0xe0
    float field_0xe4;                   // +0xe4
    int field_0xe8;                     // +0xe8
    bool field_0xec;                    // +0xec
    bool field_0xed;                    // +0xed
    float field_0xf0;                   // +0xf0
    longlong field_0xf8;                // +0xf8
    longlong field_0x100;               // +0x100
    longlong field_0x108;               // +0x108
    longlong field_0x110;               // +0x110
    int field_0x11c;                    // +0x11c
    int field_0x120;                    // +0x120
    bool field_0x17c;                   // +0x17c
};
#endif
