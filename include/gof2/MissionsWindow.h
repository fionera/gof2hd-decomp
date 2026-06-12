#ifndef GOF2_MISSIONSWINDOW_H
#define GOF2_MISSIONSWINDOW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- MissionsWindow (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace) -- the qualified target name is
// "MissionsWindow::...". Field offsets are recovered per-method from the target
// disassembly; we do NOT model a full layout -- fields are reached via byte-offset casts.


void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

// Field accessors via byte offset.

// 16-byte aggregate cleared in the ctor (4 ints).
#ifndef GOF2_BLK16_DEFINED
#define GOF2_BLK16_DEFINED
struct __attribute__((aligned(4))) Blk16 { int a, b, c, d; };
#endif

class MissionsWindow {
public:
    Blk16 field_0xc;                    // +0xc
    MissionsWindow();
    ~MissionsWindow();
    int OnTouchMove(int, int);
    int OnTouchBegin(int, int);
    void setHangarUpdate(bool);
    uint8_t hangarNeedsUpdate();
    void render3D();
};
#endif
