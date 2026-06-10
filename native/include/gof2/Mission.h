#ifndef GOF2_MISSION_H
#define GOF2_MISSION_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — Mission class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace). Field offsets recovered from the
// per-method target disassembly. We do NOT model a full layout: methods access
// fields via byte-offset casts from `this` (which arrives in r0 just like the
// first explicit arg of an extern "C" function).
//
// Known offsets:
//   0x00 vtable ptr        0x10 String name        0x1c String targetName
//   0x40 String targetStationName                  0x4c String targetSystemName
//   0x0c int id            0x3c int targetStation  0x58 int reward
//   0x5c bool instantAction 0x60 float distance     0x64 int campaign
//   0x68/0x6c production goods (int,int)            0x74 bool visible


struct Mission;

// AbyssEngine::String passed by value is a 12-byte aggregate. In the engine it has
// a non-trivial copy ctor/dtor, so by-value params are passed by invisible
// reference (a pointer). Model it opaquely.
using AbyssEngine::String12;

// Full Mission layout recovered from the per-method disassembly. The four embedded
// String members (0x10/0x1c/0x40/0x4c) are reached via byte-offset casts, so they are
// kept as raw 12-byte slots here.
struct Mission {
    void* field_0x0;                    // +0x0  vtable ptr
    int field_0x4;                      // +0x4
    int field_0x8;                      // +0x8
    int field_0xc;                      // +0xc  id
    unsigned char field_0x10[12];       // +0x10 String name
    unsigned char field_0x1c[12];       // +0x1c String targetName
    int field_0x28;                     // +0x28
    int field_0x2c;                     // +0x2c
    int field_0x30;                     // +0x30
    int field_0x34;                     // +0x34
    int field_0x38;                     // +0x38
    int field_0x3c;                     // +0x3c targetStation
    unsigned char field_0x40[12];       // +0x40 String targetStationName
    unsigned char field_0x4c[12];       // +0x4c String targetSystemName
    int field_0x58;                     // +0x58 reward
    uint8_t field_0x5c;                 // +0x5c instantAction
    unsigned char _pad_5d[3];
    int field_0x60;                     // +0x60 distance
    int field_0x64;                     // +0x64 campaign
    int field_0x68;                     // +0x68 production goods a
    int field_0x6c;                     // +0x6c production goods b
    int field_0x70;                     // +0x70
    uint8_t field_0x74;                 // +0x74 visible
};
#endif
