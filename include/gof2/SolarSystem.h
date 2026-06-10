#ifndef GOF2_SOLARSYSTEM_H
#define GOF2_SOLARSYSTEM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — SolarSystem class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: on the class itself), though it embeds an
// AbyssEngine::String at +0x0c. Field offsets recovered per-method from the
// target disassembly; methods access fields via byte-offset casts from `this`.


struct SolarSystem;

// AbyssEngine::String passed/returned by value is a 12-byte trivially-copied
// aggregate (text*, size, ...). Model it opaquely so the ABI matches the target.


// Array<int> header layout: { length, data, cap }.


// Field accessors via byte offset from `this`.

static inline int32_t &i32(void *self, unsigned off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, unsigned off) { return *(uint32_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, unsigned off) { return *(uint8_t *)((char *)self + off); }

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
