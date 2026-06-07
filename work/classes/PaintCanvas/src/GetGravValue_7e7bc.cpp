#include "class.h"

extern "C" void paintcanvas_ext_get_grav(void *);

void GetGravValue(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_get_grav(*(void **)((char *)self + 0x34));
}
