#ifndef GOF2_PLAYERFIGHTER_H
#define GOF2_PLAYERFIGHTER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - PlayerFighter class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace). Field offsets recovered per-method from
// the target disassembly; we do NOT model a full layout - access fields via byte-offset
// casts from `self`.


struct PlayerFighter;



// AbyssEngine::AEMath::Vector - 3 floats. Passed by const ref / value.
namespace AbyssEngine { namespace AEMath {

} }
typedef AbyssEngine::AEMath::Vector Vector;

// AbyssEngine::String - 12-byte value aggregate (text*, field, size).
namespace AbyssEngine {

}
typedef AbyssEngine::String String;

// Field accessor via byte offset.

struct PlayerFighter {
    int32_t field_0x4;                  // +0x4
    int32_t field_0x8;                  // +0x8
    int32_t field_0xc;                  // +0xc
    uint8_t field_0x24;                 // +0x24
    signed char field_0x25;             // +0x25
    int32_t field_0x28;                 // +0x28
    int32_t field_0x38;                 // +0x38
    signed char field_0x43;             // +0x43
    uint8_t field_0x4c;                 // +0x4c
    void* field_0x50;                   // +0x50
    int32_t field_0x54;                 // +0x54
    int32_t field_0x58;                 // +0x58
    int32_t field_0x5c;                 // +0x5c
    int32_t field_0x60;                 // +0x60
    uint8_t field_0x68;                 // +0x68
    uint8_t field_0x69;                 // +0x69
    uint8_t field_0x6a;                 // +0x6a
    int32_t field_0x6c;                 // +0x6c
    signed char field_0x74;             // +0x74
    int32_t field_0x78;                 // +0x78
    int32_t field_0x80;                 // +0x80
    int32_t field_0x84;                 // +0x84
    int32_t field_0x88;                 // +0x88
    uint8_t field_0xd0;                 // +0xd0
    int32_t field_0xd8;                 // +0xd8
    signed char field_0xdc;             // +0xdc
    int32_t field_0xe4;                 // +0xe4
    int32_t field_0xf0;                 // +0xf0
    uint8_t field_0xf5;                 // +0xf5
    int32_t field_0xf8;                 // +0xf8
    signed char field_0x100;            // +0x100
    int32_t field_0x104;                // +0x104
    int32_t field_0x108;                // +0x108
    float field_0x11c;                  // +0x11c
    float field_0x120;                  // +0x120
    void* field_0x124;                  // +0x124
    int32_t field_0x128;                // +0x128
    uint8_t field_0x12c;                // +0x12c
    short field_0x12d;                  // +0x12d
    uint8_t field_0x12e;                // +0x12e
    signed char field_0x12f;            // +0x12f
    int32_t field_0x130;                // +0x130
    int32_t field_0x134;                // +0x134
    int32_t field_0x138;                // +0x138
    signed char field_0x13c;            // +0x13c
    uint8_t field_0x13d;                // +0x13d
    signed char field_0x13e;            // +0x13e
    int32_t field_0x140;                // +0x140
    void* field_0x144;                  // +0x144
    int32_t field_0x148;                // +0x148
    int32_t field_0x14c;                // +0x14c
    void* field_0x150;                  // +0x150
    int32_t field_0x154;                // +0x154
    int32_t field_0x158;                // +0x158
    int32_t field_0x15c;                // +0x15c
    int32_t field_0x160;                // +0x160
    int32_t field_0x1a0;                // +0x1a0
    float field_0x1a4;                  // +0x1a4
    float field_0x1a8;                  // +0x1a8
    float field_0x1ac;                  // +0x1ac
    int32_t field_0x1b0;                // +0x1b0
    int32_t field_0x1b4;                // +0x1b4
    int32_t field_0x1b8;                // +0x1b8
    int32_t field_0x1c0;                // +0x1c0
    int32_t field_0x1c4;                // +0x1c4
    int32_t field_0x1c8;                // +0x1c8
    int32_t field_0x1d0;                // +0x1d0
    int32_t field_0x1d4;                // +0x1d4
    int32_t field_0x1d8;                // +0x1d8
    int32_t field_0x1dc;                // +0x1dc
    signed char field_0x1e0;            // +0x1e0
    int32_t field_0x1e4;                // +0x1e4
    float field_0x1e8;                  // +0x1e8
    float field_0x1f0;                  // +0x1f0
    short field_0x1f4;                  // +0x1f4
    int32_t field_0x1f8;                // +0x1f8
    signed char field_0x1fc;            // +0x1fc
    int32_t field_0x200;                // +0x200
    int32_t field_0x204;                // +0x204
    int32_t field_0x208;                // +0x208
    int32_t field_0x20c;                // +0x20c
    int32_t field_0x210;                // +0x210
    int32_t field_0x214;                // +0x214
    signed char field_0x254;            // +0x254
    signed char field_0x255;            // +0x255
    int32_t field_0x294;                // +0x294
    int32_t field_0x298;                // +0x298
    int32_t field_0x2b0;                // +0x2b0
    signed char field_0x2b4;            // +0x2b4
    void* field_0x2b8;                  // +0x2b8
    int32_t field_0x2c4;                // +0x2c4
    int32_t field_0x2c8;                // +0x2c8
    int32_t field_0x2c9;                // +0x2c9
    int32_t field_0x2cc;                // +0x2cc
    int32_t field_0x2cd;                // +0x2cd
    uint8_t field_0x2d0;                // +0x2d0
    int32_t field_0x2d4;                // +0x2d4
    uint8_t field_0x2d8;                // +0x2d8
    signed char field_0x2d9;            // +0x2d9
    uint32_t field_0x2dc;               // +0x2dc
    int32_t field_0x2e0;                // +0x2e0
    uint8_t field_0x2e4;                // +0x2e4
    int32_t field_0x2e8;                // +0x2e8
};
#endif
