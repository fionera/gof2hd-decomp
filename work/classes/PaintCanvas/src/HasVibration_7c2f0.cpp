#include "class.h"

extern "C" void paintcanvas_ext_has_vibration(void *);

void HasVibration(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_has_vibration(*(void **)((char *)self + 0x34));
}
