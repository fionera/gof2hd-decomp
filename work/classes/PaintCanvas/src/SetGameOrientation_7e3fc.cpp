#include "class.h"

__attribute__((visibility("hidden"))) extern const unsigned int g_sgo_const_8e6b4;
__attribute__((visibility("hidden"))) extern const unsigned int g_sgo_const_8e6b8;

extern "C" void paintcanvas_ext_sgo_setorientation(void *eng, int mode);
extern "C" int paintcanvas_ext_sgo_dispwidth(void *eng);
extern "C" int paintcanvas_ext_sgo_dispheight(void *eng);
extern "C" float paintcanvas_ext_sgo_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_sgo_setpersp(void *self, float a, float b, float c);

static void zero16(char *p)
{
    memset(p, 0, 0x10);
}

void SetGameOrientation(AbyssEngine::PaintCanvas *self, int param_2)
{
    char *t = (char *)self;
    if (*(int *)(t + 0x30) == param_2) {
        return;
    }
    *(int *)(t + 0x30) = param_2;
    paintcanvas_ext_sgo_setorientation(*(void **)(t + 0x34), param_2);

    *(float *)(t + 0x3c) = -*(float *)(t + 0x3c);
    *(float *)(t + 0x48) = -*(float *)(t + 0x48);

    int w = paintcanvas_ext_sgo_dispwidth(*(void **)(t + 0x34));
    float fw = paintcanvas_ext_sgo_signedtofloat(w, 0);
    int h = paintcanvas_ext_sgo_dispheight(*(void **)(t + 0x34));
    float fh = paintcanvas_ext_sgo_signedtofloat(h, 0);

    if (param_2 == 3) {
        zero16(t + 0x9c); zero16(t + 0xd8); zero16(t + 0xc8);
        zero16(t + 0x8c); zero16(t + 0x7c); zero16(t + 0xe4); zero16(t + 0xb8);
        *(unsigned int *)(t + 0xb0) = 0;
        *(unsigned int *)(t + 0xac) = 0x3f800000;
        *(unsigned int *)(t + 0xec) = 0;
        *(unsigned int *)(t + 0xf0) = 0;
        *(unsigned int *)(t + 0xb4) = 0x3f800000;
        *(unsigned int *)(t + 0xb8) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xbc) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xa0) = g_sgo_const_8e6b4;
        *(unsigned int *)(t + 0xa8) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xe0) = 0x3f800000;
        *(unsigned int *)(t + 0xc8) = 0x3f800000;
        *(float *)(t + 0x78) = 2.0f / fw;
        *(float *)(t + 0x8c) = -(2.0f / fh);
        *(unsigned int *)(t + 0xf4) = 0x3f800000;
        int w2 = paintcanvas_ext_sgo_dispwidth(*(void **)(t + 0x34));
        *(float *)(t + 0xe8) = paintcanvas_ext_sgo_signedtofloat(w2, 0);
        int h2 = paintcanvas_ext_sgo_dispheight(*(void **)(t + 0x34));
        *(float *)(t + 0xec) = paintcanvas_ext_sgo_signedtofloat(h2, 0);
    } else if (param_2 == 1) {
        zero16(t + 0x9c); zero16(t + 0xd8); zero16(t + 0xc8);
        zero16(t + 0x8c); zero16(t + 0x7c); zero16(t + 0xe4); zero16(t + 0xbc);
        *(unsigned int *)(t + 0xb0) = 0;
        *(unsigned int *)(t + 0xac) = 0x3f800000;
        *(unsigned int *)(t + 0xb4) = 0x3f800000;
        *(unsigned int *)(t + 0xbc) = 0x3f800000;
        *(unsigned int *)(t + 0xa0) = g_sgo_const_8e6b4;
        *(unsigned int *)(t + 0xa8) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xe0) = 0x3f800000;
        *(unsigned int *)(t + 0xc8) = g_sgo_const_8e6b8;
        *(float *)(t + 0x78) = 2.0f / fh;
        *(float *)(t + 0x8c) = -(2.0f / fw);
        *(unsigned int *)(t + 0xf4) = 0x3f800000;
        int h2 = paintcanvas_ext_sgo_dispheight(*(void **)(t + 0x34));
        *(float *)(t + 0xe8) = paintcanvas_ext_sgo_signedtofloat(h2, 0);
    } else if (param_2 != 0) {
        zero16(t + 0x9c); zero16(t + 0xd8); zero16(t + 0xc8);
        zero16(t + 0x8c); zero16(t + 0x7c); zero16(t + 0xbc);
        *(unsigned int *)(t + 0xb0) = 0;
        *(unsigned int *)(t + 0xac) = 0x3f800000;
        *(unsigned int *)(t + 0xec) = 0;
        *(unsigned int *)(t + 0xf0) = 0;
        *(unsigned int *)(t + 0xb4) = 0x3f800000;
        *(unsigned int *)(t + 0xb8) = 0x3f800000;
        *(unsigned int *)(t + 0xa0) = g_sgo_const_8e6b4;
        *(unsigned int *)(t + 0xa8) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xe0) = 0x3f800000;
        *(unsigned int *)(t + 0xcc) = 0x3f800000;
        *(float *)(t + 0x78) = 2.0f / fw;
        *(float *)(t + 0x8c) = -(2.0f / fh);
        *(unsigned int *)(t + 0xf4) = 0x3f800000;
    } else {
        zero16(t + 0x9c); zero16(t + 0xd8); zero16(t + 0xc8);
        zero16(t + 0x8c); zero16(t + 0x7c); zero16(t + 0xe4); zero16(t + 0xbc);
        *(unsigned int *)(t + 0xb0) = 0;
        *(unsigned int *)(t + 0xac) = 0x3f800000;
        *(unsigned int *)(t + 0xb4) = 0x3f800000;
        *(unsigned int *)(t + 0xbc) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xa0) = g_sgo_const_8e6b4;
        *(unsigned int *)(t + 0xa8) = g_sgo_const_8e6b8;
        *(unsigned int *)(t + 0xe0) = 0x3f800000;
        *(unsigned int *)(t + 0xc8) = 0x3f800000;
        *(float *)(t + 0x78) = 2.0f / fh;
        *(float *)(t + 0x8c) = -(2.0f / fw);
        *(unsigned int *)(t + 0xf4) = 0x3f800000;
        int w2 = paintcanvas_ext_sgo_dispwidth(*(void **)(t + 0x34));
        *(float *)(t + 0xec) = paintcanvas_ext_sgo_signedtofloat(w2, 0);
    }

    if (*(int *)(t + 0x170) == -1) {
        return;
    }
    float *cam = (*(float ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
    paintcanvas_ext_sgo_setpersp(self, cam[0], cam[1], cam[2]);
}
