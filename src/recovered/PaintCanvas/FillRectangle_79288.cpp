#include "class.h"

extern "C" void paintcanvas_ext_fr_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_fr_glenable(void *eng, unsigned int cap, bool on);
extern "C" void paintcanvas_ext_fr_meshdraw(void *eng, void *mesh);

void FillRectangle(AbyssEngine::PaintCanvas *self, int x, int y, int w, int h)
{
    char abuf[60];
    float fx = (float)x;
    float fy = (float)y;
    float fx2 = (float)(x + w);
    float fy2 = (float)(h + y);

    float *vb = *(float **)(*(char **)((char *)self + 0x1c8) + 0x4);
    vb[0] = fx; vb[1] = fy;
    vb[3] = fx2; vb[4] = fy;
    vb[6] = fx2; vb[7] = fy2;
    vb[9] = fx; vb[10] = fy2;

    float *m = (float *)abuf;
    m[0] = 1.0f;
    m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
    m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
    m[14] = 1.0f;

    paintcanvas_ext_fr_setwvm(self, abuf);
    paintcanvas_ext_fr_glenable(*(void **)((char *)self + 0x34), 0xde1, false);
    paintcanvas_ext_fr_meshdraw(*(void **)((char *)self + 0x34), *(void **)((char *)self + 0x1c8));
    paintcanvas_ext_fr_glenable(*(void **)((char *)self + 0x34), 0xde1, true);
}
