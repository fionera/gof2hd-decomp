#include "class.h"

extern "C" void paintcanvas_ext_array_removeall_mesh(void *);
extern "C" void paintcanvas_ext_array_removeall_uint(void *);
extern "C" void paintcanvas_ext_array_removeall(void *);

void RemoveAllMatsForGlow(AbyssEngine::PaintCanvas *self)
{
    paintcanvas_ext_array_removeall_mesh((char *)self + 0x18c);
    paintcanvas_ext_array_removeall((char *)self + 0x198);
    paintcanvas_ext_array_removeall((char *)self + 0x1a4);
    paintcanvas_ext_array_removeall_uint((char *)self + 0x1b0);
    return paintcanvas_ext_array_removeall((char *)self + 0x1bc);
}
