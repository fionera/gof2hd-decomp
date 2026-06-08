#include "class.h"

__attribute__((visibility("hidden"))) extern const float g_di2_one_88d90;
__attribute__((visibility("hidden"))) extern const float g_di2_def_88d94;

extern "C" void paintcanvas_ext_di2_restore(unsigned int flag, void *img);
extern "C" void paintcanvas_ext_di2_settexture(void *self, unsigned int tex, int slot);
extern "C" float paintcanvas_ext_di2_signedtofloat(int v, unsigned int mode);
extern "C" float paintcanvas_ext_di2_unsignedtofloat(unsigned int v, unsigned int mode);
extern "C" void paintcanvas_ext_di2_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_di2_gldisable(unsigned int cap);
extern "C" void paintcanvas_ext_di2_meshdraw(void *eng, void *mesh);
extern "C" void paintcanvas_ext_di2_glenable(unsigned int cap);

void DrawImage2D(AbyssEngine::PaintCanvas *self, unsigned int param_1, int param_2, int param_3,
                 unsigned char param_4)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x14c)) {
        return;
    }
    char *img = (*(char ***)(t + 0x150))[param_1];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_di2_restore(*(unsigned char *)(img + 0x14), img);
        img = (*(char ***)(t + 0x150))[param_1];
    }
    paintcanvas_ext_di2_settexture(self, *(unsigned int *)(img + 4), -1);

    float fx = paintcanvas_ext_di2_signedtofloat(param_2, 0);
    float fy = paintcanvas_ext_di2_signedtofloat(param_3, 0);

    float m[16];
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;

    if (param_4 & 1) {
        m[0] = g_di2_one_88d90;
        float off = g_di2_def_88d94;
        if (param_1 < *(unsigned int *)(t + 0x14c)) {
            unsigned short w = *(unsigned short *)((*(char ***)(t + 0x150))[param_1] + 0x10);
            off = paintcanvas_ext_di2_unsignedtofloat(w, 0);
        }
        fx = off + fx;
    }
    if (param_4 & 2) {
        m[5] = g_di2_one_88d90;
        float off = g_di2_def_88d94;
        if (param_1 < *(unsigned int *)(t + 0x14c)) {
            unsigned short h = *(unsigned short *)((*(char ***)(t + 0x150))[param_1] + 0x12);
            off = paintcanvas_ext_di2_unsignedtofloat(h, 0);
        }
        fy = off + fy;
    }
    m[3] = fx;
    m[7] = fy;

    paintcanvas_ext_di2_setwvm(self, m);
    paintcanvas_ext_di2_gldisable(0xb44);
    paintcanvas_ext_di2_meshdraw(*(void **)(t + 0x34),
                                 *(void **)((*(char ***)(t + 0x150))[param_1]));
    paintcanvas_ext_di2_glenable(0xb44);
}
