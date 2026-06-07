#include "class.h"

extern "C" void paintcanvas_ext_setcolor(void *, float, float, float, float);

void SetColor(AbyssEngine::PaintCanvas *self, unsigned char r, unsigned char g,
              unsigned char b, unsigned char a)
{
    float fr = (float)((double)(unsigned int)r / 255.0);
    float fg = (float)((double)(unsigned int)g / 255.0);
    float fb = (float)((double)(unsigned int)b / 255.0);
    float fa = (float)((double)(unsigned int)a / 255.0);
    *(float *)((char *)self + 0x1fc) = fr;
    *(float *)((char *)self + 0x200) = fg;
    *(float *)((char *)self + 0x204) = fb;
    *(float *)((char *)self + 0x208) = fa;
    return paintcanvas_ext_setcolor(*(void **)((char *)self + 0x34), fr, fg, fb, fa);
}
