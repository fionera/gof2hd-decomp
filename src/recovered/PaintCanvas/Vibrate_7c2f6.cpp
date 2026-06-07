#include "class.h"

extern "C" void paintcanvas_ext_vibrate(void *);

void Vibrate(AbyssEngine::PaintCanvas *self, unsigned short)
{
    return paintcanvas_ext_vibrate(*(void **)((char *)self + 0x34));
}
