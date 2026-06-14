#ifndef GOF2_PARTICLESETTINGS_H
#define GOF2_PARTICLESETTINGS_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — ParticleSettings class (Android libgof2hdaa.so, armv7 Thumb).
// It is an array of 48 "ParticleSet" entries, each 0xa0 bytes, laid out contiguously
// from offset 0 (total 0x1e00 bytes). Every ParticleSet begins with an
// AbyssEngine::String, so the embedded String members auto construct/destruct. The
// large init() data-table initialiser writes individual ParticleSet entries through
// named members (sets[index].field_0xNN).

// One particle-emitter parameter block (0xa0 bytes). Trailing comments are the original
// 32-bit field offsets, kept for cross-reference; the live layout is natural 64-bit.
// The initialiser performed several NEON 64-bit (d-register) pair-stores that, in the
// original byte layout, wrote two adjacent 32-bit float slots at once; those are split
// into two named-member writes here. Every accessed slot has its own named member.
struct ParticleSet {
    String   name;          // +0x00  emitter/texture name (embedded AbyssEngine::String)
    uint32_t field_0xc;     // +0x0c
    uint32_t field_0x10;    // +0x10
    uint32_t field_0x14;    // +0x14  (float)
    uint32_t field_0x18;    // +0x18
    uint32_t field_0x1c;    // +0x1c
    uint32_t field_0x20;    // +0x20  (float)
    uint32_t field_0x24;    // +0x24
    uint32_t field_0x28;    // +0x28  count (int, blended in float space by Interpolate)
    uint32_t field_0x2c;    // +0x2c
    uint32_t field_0x30;    // +0x30
    uint32_t field_0x34;    // +0x34
    uint32_t field_0x38;    // +0x38
    uint32_t field_0x3c;    // +0x3c
    uint32_t field_0x40;    // +0x40
    uint32_t field_0x44;    // +0x44
    uint32_t field_0x48;    // +0x48
    uint32_t field_0x4c;    // +0x4c
    uint32_t field_0x50;    // +0x50
    uint32_t field_0x54;    // +0x54
    uint32_t field_0x58;    // +0x58
    uint32_t field_0x5c;    // +0x5c
    uint32_t field_0x60;    // +0x60
    uint32_t field_0x64;    // +0x64
    uint32_t field_0x68;    // +0x68
    uint32_t field_0x6c;    // +0x6c
    uint32_t field_0x70;    // +0x70  (float)
    uint32_t field_0x74;    // +0x74
    uint32_t field_0x78;    // +0x78
    uint32_t field_0x7c;    // +0x7c
    uint32_t field_0x80;    // +0x80
    uint32_t field_0x84;    // +0x84
    uint32_t field_0x88;    // +0x88
    uint32_t field_0x8c;    // +0x8c
    uint32_t field_0x90;    // +0x90
    uint32_t field_0x94;    // +0x94
    uint32_t field_0x98;    // +0x98
    uint32_t field_0x9c;    // +0x9c
};

class ParticleSettings {
public:
    ParticleSet sets[48];    // 48 * 0xa0 == 0x1e00 bytes

    ParticleSettings();
    ~ParticleSettings();

    // ---- methods (converted from free functions) ----
    int init();
};
#endif
