#include "class.h"

__attribute__((visibility("hidden"))) char *paintcanvas_g_bg_flag;

void EndBG(AbyssEngine::PaintCanvas *self)
{
    *paintcanvas_g_bg_flag = *(unsigned char *)((char *)self + 0x1f0);
}
