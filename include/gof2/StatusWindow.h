#ifndef GOF2_STATUSWINDOW_H
#define GOF2_STATUSWINDOW_H
#include "gof2/common.h"
// ImageFactory.h provides the shared `struct Arr` { size, data, size2 } view; reuse it here so
// the two definitions don't collide at global scope when both headers are pulled into a TU.
#include "gof2/ImageFactory.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- StatusWindow (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name is top-level: "StatusWindow::..." (class not in a namespace;
// argument types like AbyssEngine::String / Array<...> are namespaced/templated).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.
//
// Partial layout (from dtor/scroll/touch decompiles):
//   +0x04  Array<TouchButton*>*
//   +0x08  Array<TouchButton*>*
//   +0x0c  Array<ImagePart*>*
//   +0x10  Array<AbyssEngine::String*>*
//   +0x38  int   scroll range
//   +0x58  int   content height-ish (a)
//   +0x5c  int   content height-ish (b)


struct StatusWindow;

typedef AbyssEngine::String String;

// Field accessors via byte offset.


// Engine Array<T> header layout { length, data, cap } — `struct Arr` comes from ImageFactory.h.

struct StatusWindow {
    Arr* tabButtons;                     // +0x4
    Arr* medalButtons;                     // +0x8
    void* field_0xc;                    // +0xc
    void* detailLines;                   // +0x10
    unsigned char isDragging;           // +0x54

    // ---- methods (converted from free functions) ----
    int OnTouchBegin(int param_1, int param_2);
    void OnTouchEnd(int x, int y);
    int OnTouchMove(int param_1, int param_2);
    StatusWindow * ctor();
    void draw();
    float getRelativeScrollHeight();
    float getRelativeScrollStartPos();
    void reInit();
    void update();
};
#endif
