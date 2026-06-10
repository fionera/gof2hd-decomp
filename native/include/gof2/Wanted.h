#ifndef GOF2_WANTED_H
#define GOF2_WANTED_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — Wanted class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace). Field offsets recovered from the
// per-method target disassembly. We do NOT model a full layout: methods access
// fields via byte-offset casts from `this` (which arrives in r0 just like the
// first explicit arg of an extern "C" function).


struct Wanted;

// AbyssEngine::String passed by value is a 12-byte trivially-copied aggregate
// (text*, size, ...). Model it opaquely so it is passed the same way as the
// target ABI (struct > 8 bytes by value -> on the stack / sret-style).


// Field accessor via byte offset.

struct Wanted {
    uint8_t field_0x50;                 // +0x50
    uint8_t field_0x51;                 // +0x51
};
#endif
