#include "class.h"

extern "C" void paintcanvas_ext_get_accel(void *);

void GetAccelValue(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_get_accel(*(void **)((char *)self + 0x34));
}
