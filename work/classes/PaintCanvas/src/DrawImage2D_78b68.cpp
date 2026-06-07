#include "class.h"

extern "C" void paintcanvas_ext_di_restore(unsigned int flag, void *img);
extern "C" void paintcanvas_ext_di_settexture(void *self, unsigned int tex, int slot);
extern "C" void paintcanvas_ext_di_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_di_gldisable(unsigned int cap);
extern "C" void paintcanvas_ext_di_meshdraw(void *eng, void *mesh);
extern "C" void paintcanvas_ext_di_glenable(unsigned int cap);

void DrawImage2D(AbyssEngine::PaintCanvas *self, unsigned int index, int x, int y)
{
    char abuf[60];
    if (index < *(unsigned int *)((char *)self + 0x14c)) {
        char *img = (*(char ***)((char *)self + 0x150))[index];
        if (*(unsigned char *)(img + 0x14)) {
            paintcanvas_ext_di_restore(*(unsigned char *)(img + 0x14), img);
            img = (*(char ***)((char *)self + 0x150))[index];
        }
        paintcanvas_ext_di_settexture(self, *(unsigned int *)(img + 0x4), -1);

        float fx = (float)x;
        float fy = (float)y;
        float *m = (float *)abuf;
        m[0] = 1.0f;
        m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
        m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
        m[14] = 1.0f;
        m[3] = fx;
        m[7] = fy;

        paintcanvas_ext_di_setwvm(self, abuf);
        paintcanvas_ext_di_gldisable(0xb44);
        paintcanvas_ext_di_meshdraw(*(void **)((char *)self + 0x34),
                                    *(void **)((*(char ***)((char *)self + 0x150))[index]));
        paintcanvas_ext_di_glenable(0xb44);
    }
}
