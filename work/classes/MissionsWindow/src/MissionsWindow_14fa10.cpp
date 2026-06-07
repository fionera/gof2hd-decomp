#include "class.h"

extern "C" int   _mw_GetTextHeight(void *canvas);   // 0x6fe20
extern "C" void *_mw_init(void *self);              // 0x753d0

// Two PC-relative singletons; each holds a pointer (read via one extra deref).
// The second is loaded and dereferenced but its value is unused in the target,
// so we read it through a volatile lvalue to keep the load from being elided.
__attribute__((visibility("hidden"))) extern void **g_mw_a;
__attribute__((visibility("hidden"))) extern void **g_mw_b;

struct __attribute__((aligned(4))) Blk16 { int a, b, c, d; };

// MissionsWindow::MissionsWindow()
struct MissionsWindow { MissionsWindow(); };
MissionsWindow::MissionsWindow()
{
    void *canvas = *g_mw_a;
    void *dead = *(void *volatile *)*g_mw_b;
    i32(this, 0x24) = 0;
    i32(this, 0x28) = 0;
    i32(this, 0x2c) = 0;
    Blk16 z = {0, 0, 0, 0};
    *(Blk16 *)((char *)this + 0xc) = z;
    *(Blk16 *)this = z;
    (void)dead;
    int h = _mw_GetTextHeight(canvas);
    i32(this, 0x1c) = h / 2 - 1;
    _mw_init(this);
}
