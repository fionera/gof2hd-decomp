#include "class.h"

extern "C" void paintcanvas_ext_set_wvm2(void *self, void *m);
extern "C" void paintcanvas_ext_meshdraw(void *engine, void *mesh);

void End2d(AbyssEngine::PaintCanvas *self)
{
    char buf[60];
    int v = *(int *)((char *)self + 0xc);
    if (v >= 1) {
        float *m = (float *)buf;
        *(short *)(*(char **)((char *)self + 0x8) + 0x28) = (short)(v * 6);
        m[0] = 1.0f;
        m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
        m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
        m[14] = 1.0f;
        paintcanvas_ext_set_wvm2(self, buf);
        paintcanvas_ext_meshdraw(*(void **)((char *)self + 0x34), *(void **)((char *)self + 0x8));
    }
}
