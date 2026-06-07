#include "class.h"

extern "C" void _mw_WantedWindow_render3D(void *ww);   // 0x772b4
extern "C" void _mw_render3D_tail(void *obj);          // 0x1ac258 tail

// MissionsWindow::render3D()
struct MissionsWindow { void render3D(); };
void MissionsWindow::render3D()
{
    if (i32(this, 0x40) == 1)
        _mw_WantedWindow_render3D(pp(this, 0x10));
    if (u8(this, 0x22))
        return _mw_render3D_tail(pp(this, 0x8));
}
