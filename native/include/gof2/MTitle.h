#ifndef GOF2_MTITLE_H
#define GOF2_MTITLE_H
#include "gof2/common.h"
// MTitle — title screen. Top-level class (NO namespace).
// Fields recovered from per-method byte-offset access; named field_0xNN.

struct MTitle {
    void* field_0x0;                    // +0x0   vtable pointer
    void* field_0x4;                    // +0x4   PaintCanvas
    void* field_0x8;                    // +0x8   ApplicationManager
    int field_0xc;                      // +0xc   render priority (=100)
    uint32_t field_0x10;                // +0x10  image handle
    uint32_t field_0x14;                // +0x14  image handle
    int field_0x18;                     // +0x18  step
    int field_0x1c;                     // +0x1c  timer accumulator

    MTitle();
    void OnRelease();
    void OnRender2D();
    void OnRender3D();
    void OnTouchEnd(int x, int y);
    int OnInitialize();
};
#endif
