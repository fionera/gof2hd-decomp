#include "class.h"

extern "C" void paintcanvas_ext_get_height(void *);

void GetHeight(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_get_height(*(void **)((char *)self + 0x34));
}
