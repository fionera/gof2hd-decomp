#ifndef GOF2_PARTICLESYSTEMSPRITE_H
#define GOF2_PARTICLESYSTEMSPRITE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct ParticleSystemSprite {
    uint8_t field_0x4;                  // +0x4
    unsigned int field_0x8;             // +0x8
    unsigned char field_0x34;           // +0x34
    unsigned char field_0x37;           // +0x37
    char field_0x45;                    // +0x45
    int field_0x48;                     // +0x48
    int field_0x50;                     // +0x50
    unsigned int field_0x54;            // +0x54
    unsigned int field_0x58;            // +0x58
    uint8_t field_0x5c;                 // +0x5c
    int field_0x60;                     // +0x60
    void* field_0x64;                   // +0x64
    void* field_0x68;                   // +0x68
    void* field_0x6c;                   // +0x6c
    float field_0x70;                   // +0x70
};
#endif
