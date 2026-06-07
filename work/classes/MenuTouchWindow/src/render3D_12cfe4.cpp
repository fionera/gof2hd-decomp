#include "class.h"

extern "C" void _mtw_render3D_inner(void *obj);

// MenuTouchWindow::render3D()
struct MenuTouchWindow { void render3D(); };
void MenuTouchWindow::render3D()
{
    if (i32(this, 0x16c) != 9)
        return;
    void *obj = pp(this, 0xfc);
    if (obj == 0)
        return;
    return _mtw_render3D_inner(obj);
}
