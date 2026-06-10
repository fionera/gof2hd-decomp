#ifndef GOF2_HANGARWINDOW_H
#define GOF2_HANGARWINDOW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - HangarWindow class. Android libgof2hdaa.so, armv7 Thumb.
// Top-level class (no AbyssEngine namespace). Field offsets recovered per-method from the
// target disassembly; we do NOT model a full layout - access fields via byte-offset casts.


// Opaque referenced types.
struct HangarWindow;
struct HangarList;
struct ListItem;
struct Item;
struct BluePrint;
struct Station;



// AbyssEngine::AEMath::Vector - 3 floats.
namespace AbyssEngine { namespace AEMath {

} }
typedef AbyssEngine::AEMath::Vector Vector;

// Field accessors via byte offset.

struct HangarWindow {
    uint8_t field_0x0;                  // +0x0
    void* field_0x4;                    // +0x4
    int field_0x8;                      // +0x8
    uint8_t field_0xc;                  // +0xc
    void* field_0x10;                   // +0x10
    void* field_0x14;                   // +0x14
    void* field_0x18;                   // +0x18
    void* field_0x1c;                   // +0x1c
    void* field_0x20;                   // +0x20
    void* field_0x24;                   // +0x24
    void* field_0x28;                   // +0x28
    void* field_0x2c;                   // +0x2c
    void* field_0x30;                   // +0x30
    uint8_t field_0x3c;                 // +0x3c
    int field_0x40;                     // +0x40
    int field_0x44;                     // +0x44
    int field_0x48;                     // +0x48
    int field_0x4c;                     // +0x4c
    int field_0x50;                     // +0x50
    void* field_0x54;                   // +0x54
    int field_0x58;                     // +0x58
    void* field_0x68;                   // +0x68
    int field_0x6c;                     // +0x6c
    int field_0x70;                     // +0x70
    int field_0x78;                     // +0x78
    unsigned int field_0x7c;            // +0x7c
    void* field_0x80;                   // +0x80
    void* field_0x84;                   // +0x84
    uint8_t field_0x88;                 // +0x88
    uint8_t field_0x89;                 // +0x89
    int field_0x8c;                     // +0x8c
    uint16_t field_0x90;                // +0x90
    uint8_t field_0x91;                 // +0x91
    uint8_t field_0x92;                 // +0x92
    uint8_t field_0x93;                 // +0x93
    int field_0x94;                     // +0x94
    int field_0x98;                     // +0x98
    int field_0x9c;                     // +0x9c
    int field_0xa0;                     // +0xa0
    int field_0xa4;                     // +0xa4
    int field_0xa8;                     // +0xa8
    uint8_t field_0xac;                 // +0xac
    uint8_t field_0xad;                 // +0xad
    uint8_t field_0xae;                 // +0xae
    uint8_t field_0xaf;                 // +0xaf
    uint8_t field_0xb0;                 // +0xb0
    uint8_t field_0xb1;                 // +0xb1
    int field_0xb4;                     // +0xb4
    int field_0xb8;                     // +0xb8
    int field_0xbc;                     // +0xbc
    int field_0xc0;                     // +0xc0
    int field_0xc1;                     // +0xc1
    float field_0xc4;                   // +0xc4
    int field_0xc5;                     // +0xc5
    float field_0xc8;                   // +0xc8
    int field_0xc9;                     // +0xc9
    int field_0xcc;                     // +0xcc
    int field_0xcd;                     // +0xcd
    uint8_t field_0xd0;                 // +0xd0
    uint8_t field_0xd1;                 // +0xd1
    uint8_t field_0xd2;                 // +0xd2
    int field_0xd4;                     // +0xd4
    int field_0xd8;                     // +0xd8
    int field_0xdc;                     // +0xdc
    int field_0xe0;                     // +0xe0
    int field_0xe4;                     // +0xe4
    int field_0xf0;                     // +0xf0
    int field_0xf4;                     // +0xf4
    uint8_t field_0xf8;                 // +0xf8
    unsigned int field_0xfc;            // +0xfc
    Blk16 field_0x100;                  // +0x100
    int field_0x10c;                    // +0x10c
    int field_0x110;                    // +0x110
    int field_0x114;                    // +0x114
    int field_0x118;                    // +0x118
    uint8_t field_0x11c;                // +0x11c
    uint8_t field_0x11d;                // +0x11d
    uint8_t field_0x11e;                // +0x11e
    uint8_t field_0x11f;                // +0x11f
    int field_0x120;                    // +0x120
    int field_0x124;                    // +0x124
    int field_0x128;                    // +0x128
    int field_0x12c;                    // +0x12c
    uint8_t field_0x130;                // +0x130
};
#endif
