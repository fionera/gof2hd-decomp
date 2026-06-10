#ifndef GOF2_KIPLAYER_H
#define GOF2_KIPLAYER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct KIPlayer {
    void* field_0x4;                    // +0x4
    void* field_0x8;                    // +0x8
    void* field_0xc;                    // +0xc
    uint8_t field_0x25;                 // +0x25
    int field_0x28;                     // +0x28
    uint16_t field_0x42;                // +0x42
    void* field_0x50;                   // +0x50
    float field_0x58;                   // +0x58
    float field_0x5c;                   // +0x5c
    float field_0x60;                   // +0x60
    uint8_t field_0x69;                 // +0x69
    uint8_t field_0x6a;                 // +0x6a
    Route* field_0x6c;                  // +0x6c
    uint8_t field_0x72;                 // +0x72
    uint16_t field_0x73;                // +0x73
    uint8_t field_0x76;                 // +0x76
    void* field_0x78;                   // +0x78
    int field_0x7c;                     // +0x7c
    int field_0x88;                     // +0x88
    float field_0xa8;                   // +0xa8
    int field_0xac;                     // +0xac
    uint8_t field_0xb1;                 // +0xb1
    uint8_t field_0xb2;                 // +0xb2
    Route* field_0xb4;                  // +0xb4
    void* field_0xc4;                   // +0xc4
    void* field_0xcc;                   // +0xcc
    uint32_t field_0xd4;                // +0xd4
    uint8_t field_0xdc;                 // +0xdc
    int field_0xe0;                     // +0xe0
    int field_0xe4;                     // +0xe4
    KIPlayer* field_0xe8;               // +0xe8
    uint8_t field_0xec;                 // +0xec
    uint8_t field_0xf4;                 // +0xf4
    uint8_t field_0xf5;                 // +0xf5
    int field_0xf8;                     // +0xf8
    uint32_t field_0xfc;                // +0xfc
    uint8_t field_0x100;                // +0x100
    uint8_t field_0x101;                // +0x101
};
#endif
