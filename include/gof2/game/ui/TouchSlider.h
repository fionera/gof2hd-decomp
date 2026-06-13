#ifndef GOF2_TOUCHSLIDER_H
#define GOF2_TOUCHSLIDER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- TouchSlider (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "TouchSlider::..." (NOT in a namespace).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.


struct TouchSlider;

// Templated byte-offset field accessor: F<int>(this, 0x14) etc.

class TouchSlider {
public:
    int x;                      // +0x0
    int y;                      // +0x4
    int knobX;                      // +0x8
    int knobY;                      // +0xc
    int type;                     // +0x10
    int knobWidth;                     // +0x14
    int knobHeight;                     // +0x18
    int trackWidth;                     // +0x1c
    int trackHeight;                     // +0x20
    float value;                   // +0x24
    int numSteps;                     // +0x28
    int trackImage;                     // +0x2c
    int knobImage;                     // +0x30
    uint8_t isDragging;                 // +0x34
    uint8_t isDisabled;                 // +0x35
    int touchPadding;                     // +0x38

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
