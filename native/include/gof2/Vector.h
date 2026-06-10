#ifndef GOF2_VECTOR_H
#define GOF2_VECTOR_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- AbyssEngine::AEMath::Vector (Android libgof2hdaa.so, armv7 Thumb).
// A 3-component float vector. The recovered disassembly for these operators sits in a
// corrupted/overlapping bad-instruction region; per coverage mode we author the faithful
// component-wise semantics implied by each operator's signature and the s0/s2/s4 + [0,4,8]
// VFP load/store pattern (three contiguous floats).


namespace AbyssEngine {
namespace AEMath {



} // namespace AEMath
} // namespace AbyssEngine

struct Vector {
    uint32_t field_0x0;                 // +0x0
    uint16_t field_0x2;                 // +0x2
    void* field_0x4;                    // +0x4
    void* field_0x8;                    // +0x8
    void* field_0xc;                    // +0xc
    void* field_0x10;                   // +0x10
    uint16_t field_0x28;                // +0x28
    int16_t* field_0x2c;                // +0x2c
};
#endif
