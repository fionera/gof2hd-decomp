#ifndef GOF2_PLAYERFIXEDOBJECT_H
#define GOF2_PLAYERFIXEDOBJECT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - PlayerFixedObject class. Android libgof2hdaa.so, armv7 Thumb.
// Top-level class (no AbyssEngine namespace). Field offsets recovered per-method from the
// target disassembly; we do NOT model a full layout - access fields via byte-offset casts.


// Opaque referenced types.
struct PlayerFixedObject;
struct BoundingVolume;
struct AEGeometry;
struct Player;



// AbyssEngine::AEMath::Vector - 3 floats. Passed by const ref / value.
namespace AbyssEngine { namespace AEMath {

} }
typedef AbyssEngine::AEMath::Vector Vector;

// Field accessors via byte offset.

struct PlayerFixedObject {
    void* field_0x4;                    // +0x4
    void* field_0x8;                    // +0x8
    unsigned char field_0x24;           // +0x24
    int field_0x28;                     // +0x28
    float field_0x2c;                   // +0x2c
    float field_0x30;                   // +0x30
    float field_0x34;                   // +0x34
    uint8_t field_0x40;                 // +0x40
    uint8_t field_0x41;                 // +0x41
    unsigned char field_0x4c;           // +0x4c
    void* field_0x50;                   // +0x50
    void* field_0x54;                   // +0x54
    float field_0x58;                   // +0x58
    float field_0x5c;                   // +0x5c
    float field_0x60;                   // +0x60
    void* field_0x78;                   // +0x78
    int field_0x88;                     // +0x88
    uint8_t field_0x8c;                 // +0x8c
    float field_0x90;                   // +0x90
    float field_0x94;                   // +0x94
    float field_0x98;                   // +0x98
    int field_0xac;                     // +0xac
    int field_0xd8;                     // +0xd8
    int32_t field_0xf8;                 // +0xf8
    unsigned char field_0x101;          // +0x101
    void* field_0x124;                  // +0x124
    void* field_0x128;                  // +0x128
    void* field_0x12c;                  // +0x12c
    int32_t field_0x130;                // +0x130
    unsigned char field_0x134;          // +0x134
    uint64_t field_0x138;               // +0x138
    uint64_t field_0x140;               // +0x140
    unsigned int field_0x144;           // +0x144
    uint64_t field_0x148;               // +0x148
    unsigned int field_0x14c;           // +0x14c
    uint64_t field_0x150;               // +0x150
    float field_0x154;                  // +0x154
    uint64_t field_0x158;               // +0x158
    uint64_t field_0x160;               // +0x160
    int32_t field_0x168;                // +0x168
    int field_0x16c;                    // +0x16c
    uint8_t field_0x170;                // +0x170
    int32_t field_0x174;                // +0x174
    int32_t field_0x178;                // +0x178
    int32_t field_0x17c;                // +0x17c
    int32_t field_0x180;                // +0x180
    uint16_t field_0x184;               // +0x184
    void* field_0x18c;                  // +0x18c
    int field_0x190;                    // +0x190
    int field_0x194;                    // +0x194
    int field_0x198;                    // +0x198
    float field_0x19c;                  // +0x19c
    int field_0x1a0;                    // +0x1a0
    int field_0x1a4;                    // +0x1a4
    int field_0x1a8;                    // +0x1a8
    uint8_t field_0x1b8;                // +0x1b8
};
#endif
