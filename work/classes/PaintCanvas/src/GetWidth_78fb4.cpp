#include "class.h"

extern "C" void paintcanvas_ext_get_width(void *);

void GetWidth(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_get_width(*(void **)((char *)self + 0x34));
}
