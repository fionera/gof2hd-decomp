#ifndef GOF2_PARTICLESETTINGS_H
#define GOF2_PARTICLESETTINGS_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — ParticleSettings class (Android libgof2hdaa.so, armv7 Thumb).
// It is an array of 48 "ParticleSet" entries, each 0xa0 bytes, laid out contiguously
// from offset 0 (total 0x1e00 bytes). Every ParticleSet begins with an
// AbyssEngine::String, so the ctor/dtor walk the array in 0xa0 strides constructing /
// destructing those strings. The large init() data-table writes address individual
// ParticleSet entries by computed pointer (entry base = sets + index).

// One particle-emitter parameter block (0xa0 bytes). Field offsets are preserved in the
// field_0xNN names; the data-table initialiser addresses these by computed pointer.
struct ParticleSet {
    String field_0x0;        // emitter/texture name (0xc bytes)
    uint32_t field_0xc;
    uint32_t field_0x10;
    float field_0x14;
    uint32_t field_0x18;
    uint32_t field_0x1c;
    uint32_t field_0x20;
    uint32_t field_0x24;
    uint32_t field_0x28;     // count (int, blended in float space by Interpolate)
    uint32_t field_0x2c;
    uint64_t field_0x30;
    uint64_t field_0x38;
    uint32_t field_0x40;
    uint32_t field_0x44;
    uint8_t field_0x48[0x48]; // cleared block (0x48..0x8f)
    uint32_t field_0x90;
    uint32_t field_0x94;
    uint32_t field_0x98;
    uint32_t field_0x9c;
};

struct ParticleSettings {
    ParticleSet sets[48];    // 48 * 0xa0 == 0x1e00 bytes

    ParticleSettings();
    ~ParticleSettings();
};
#endif
