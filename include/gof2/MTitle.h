#ifndef GOF2_MTITLE_H
#define GOF2_MTITLE_H
#include "gof2/common.h"
// MTitle — title screen. Top-level class (NO namespace).
// Fields recovered from per-method byte-offset access; named field_0xNN.

class MTitle {
public:
    void* field_0x0;                    // +0x0   vtable pointer
    void* canvas;                    // +0x4   PaintCanvas
    void* appManager;                    // +0x8   ApplicationManager
    int renderPriority;                      // +0xc   render priority (=100)
    uint32_t logoImage;                // +0x10  image handle
    uint32_t logoImage2;                // +0x14  image handle
    int step;                     // +0x18  step
    int timer;                     // +0x1c  timer accumulator

    MTitle();
    void OnRelease();
    void OnRender2D();
    void OnRender3D();
    void OnTouchEnd(int x, int y);
    int OnInitialize();
};
#endif
