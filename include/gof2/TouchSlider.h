#ifndef GOF2_TOUCHSLIDER_H
#define GOF2_TOUCHSLIDER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- TouchSlider (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "TouchSlider::..." (NOT in a namespace).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.


struct TouchSlider;

// Templated byte-offset field accessor: F<int>(this, 0x14) etc.

struct TouchSlider {
    int field_0x0;                      // +0x0
    int field_0x4;                      // +0x4
    int field_0x8;                      // +0x8
    int field_0xc;                      // +0xc
    int field_0x10;                     // +0x10
    int field_0x14;                     // +0x14
    int field_0x18;                     // +0x18
    int field_0x1c;                     // +0x1c
    int field_0x20;                     // +0x20
    float field_0x24;                   // +0x24
    int field_0x28;                     // +0x28
    int field_0x2c;                     // +0x2c
    int field_0x30;                     // +0x30
    uint8_t field_0x34;                 // +0x34
    uint8_t field_0x35;                 // +0x35
    int field_0x38;                     // +0x38

    TouchSlider(int param_1, int param_2, int param_3, float param_4);
    void setPosition(int param_1, int param_2);
    int OnTouchBegin(int param_1, int param_2);
    int OnTouchEnd(int param_1, int param_2);
    bool OnTouchMove(int param_1, int param_2);
    void draw();
    float getValue();
    void setValue(float param_1);
    void setHalfTransparent(bool param_1);
    int touchedInside(int param_1, int param_2);
};
#endif
