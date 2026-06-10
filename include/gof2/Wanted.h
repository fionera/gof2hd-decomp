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

// The recovered code uses the bare name String12 for the 12-byte stack String
// temp. It lives in namespace AbyssEngine (see common.h); pull it into scope.
using AbyssEngine::String12;

// 12-byte by-value String return aggregate (sret on the target ABI): getName()
// returns a 12-byte String temp. Modeled the same way as in Agent.h/Mission.h.

// Field accessor via byte offset.

struct Wanted {
    String field_0x0;                   // +0x0  (name, AbyssEngine::String)
    int field_0xc;                      // +0xc
    int field_0x10;                     // +0x10
    int field_0x14;                     // +0x14
    uint8_t field_0x18;                 // +0x18
    int field_0x1c;                     // +0x1c
    int field_0x20;                     // +0x20
    int field_0x24;                     // +0x24
    int field_0x28;                     // +0x28
    int field_0x2c;                     // +0x2c
    int field_0x30;                     // +0x30
    int field_0x34;                     // +0x34
    int field_0x38;                     // +0x38
    int field_0x3c;                     // +0x3c
    void* field_0x40;                   // +0x40  (heap buffer freed in dtor)
    int field_0x44;                     // +0x44
    int field_0x48;                     // +0x48
    int field_0x4c;                     // +0x4c
    uint8_t field_0x50;                 // +0x50
    uint8_t field_0x51;                 // +0x51

    // ---- methods (converted from free functions) ----
    Wanted * ctor(int p1, const String12 &p2, int p3, int p4, bool p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14);
    Wanted * dtor();
    RetStr getName();
    uint8_t isActive();
    uint8_t isTerminated();
    void setActive(bool v);
    void setTerminated(bool v);
};
#endif
