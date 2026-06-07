#include "class.h"

extern "C" void paintcanvas_ext_enable(int);
extern "C" void paintcanvas_ext_depthmask(int);
extern "C" void paintcanvas_ext_clear2(void *, unsigned int);

void ClearBuffer(AbyssEngine::PaintCanvas *self, unsigned int mask)
{
    paintcanvas_ext_enable(0xb71);
    paintcanvas_ext_depthmask(1);
    return paintcanvas_ext_clear2(*(void **)((char *)self + 0x34), mask);
}
