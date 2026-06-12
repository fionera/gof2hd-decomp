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

    // Tail fragments of the methods above (interworking-return veneers in the
    // binary that resolve to a single concrete engine call each).
    void or_tail(void *layout);             // OnRelease tail: ImageFactory/Layout reload completion
    void r2dDone(void *app, int moduleId);  // OnRender2D: logo sequence finished -> switch module
    void r2dTail(void *canvas);             // OnRender2D tail: PaintCanvas::End2d completion
    void r3dTail(void *canvas);             // OnRender3D tail: PaintCanvas::Begin3d completion
    void deleteTail();                      // deleting-dtor tail: operator delete(this)
};
#endif
