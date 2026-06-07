#include "class.h"

extern "C" void paintcanvas_ext_setcolor(void *, float, float, float, float);

void SetColor(AbyssEngine::PaintCanvas *self, unsigned int color)
{
    float c0 = (float)((double)(color >> 24) / 255.0);
    float c1 = (float)((double)((color >> 16) & 0xff) / 255.0);
    float c2 = (float)((double)((color >> 8) & 0xff) / 255.0);
    float c3 = (float)((double)(color & 0xff) / 255.0);
    *(float *)((char *)self + 0x1fc) = c0;
    *(float *)((char *)self + 0x200) = c1;
    *(float *)((char *)self + 0x204) = c2;
    *(float *)((char *)self + 0x208) = c3;
    return paintcanvas_ext_setcolor(*(void **)((char *)self + 0x34), c0, c1, c2, c3);
}
