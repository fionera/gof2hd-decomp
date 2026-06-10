#ifndef GOF2_IPARTICLESYSTEM_H
#define GOF2_IPARTICLESYSTEM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct IParticleSystem {
    void* field_0x0;                    // +0x0
    volatile uint16_t field_0x4;        // +0x4
    volatile uint8_t field_0x5;         // +0x5
    PaintCanvas* field_0x8;             // +0x8
    Matrix const* field_0x18;           // +0x18
    void* field_0x3c;                   // +0x3c
};
#endif
