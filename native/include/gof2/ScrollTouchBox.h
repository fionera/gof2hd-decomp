#ifndef GOF2_SCROLLTOUCHBOX_H
#define GOF2_SCROLLTOUCHBOX_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

}

typedef AbyssEngine::String String;
struct Array;

struct ScrollTouchBox {
    void* field_0x0;                    // +0x0
    int field_0x4;                      // +0x4
    int field_0x8;                      // +0x8
    int field_0xc;                      // +0xc
    int field_0x10;                     // +0x10
    int field_0x14;                     // +0x14
    int field_0x18;                     // +0x18
    int field_0x1c;                     // +0x1c
    float field_0x20;                   // +0x20
    float field_0x24;                   // +0x24
    int field_0x28;                     // +0x28
    volatile int field_0x29;            // +0x29
    int field_0x2c;                     // +0x2c
    volatile int field_0x2d;            // +0x2d
    uint8_t field_0x30;                 // +0x30
    int field_0x34;                     // +0x34
    uint8_t field_0x38;                 // +0x38
    String* field_0x3c;                 // +0x3c
};
#endif
