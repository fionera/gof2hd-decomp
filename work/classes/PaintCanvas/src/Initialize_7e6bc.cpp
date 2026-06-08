#include "class.h"

__attribute__((visibility("hidden"))) extern const unsigned int g_init_const_7e7b4;
__attribute__((visibility("hidden"))) extern const unsigned int g_init_const_7e7b8;

extern "C" void paintcanvas_ext_init_setorientation(void *eng);
extern "C" int paintcanvas_ext_init_dispwidth(void *eng);
extern "C" int paintcanvas_ext_init_dispheight(void *eng);
extern "C" float paintcanvas_ext_init_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_init_setpersp(void *self, float a, float b, float c);

void Initialize(AbyssEngine::PaintCanvas *self, bool param_1)
{
    char *t = (char *)self;
    *(unsigned int *)(t + 0x30) = param_1 ? 2 : 0;
    paintcanvas_ext_init_setorientation(*(void **)(t + 0x34));

    // zero out the four 4-float blocks at 0x78, 0x88, 0x98, 0xa8
    memset(t + 0x78, 0, 0x10);
    memset(t + 0x88, 0, 0x10);
    memset(t + 0x98, 0, 0x10);
    memset(t + 0xa8, 0, 0x10);

    int orient = *(int *)(t + 0x30);
    int w = paintcanvas_ext_init_dispwidth(*(void **)(t + 0x34));
    float fw = paintcanvas_ext_init_signedtofloat(w, 0);
    int h = paintcanvas_ext_init_dispheight(*(void **)(t + 0x34));
    float fh = paintcanvas_ext_init_signedtofloat(h, 0);

    float ymul;
    if (orient == 2) {
        *(float *)(t + 0x78) = 2.0f / fw;
        ymul = fh;
    } else {
        memset(t + 0xb8, 0, 0x10);
        memset(t + 0xc8, 0, 0x10);
        memset(t + 0xd8, 0, 0x10);
        memset(t + 0xe4, 0, 0x10);
        *(unsigned int *)(t + 0xf4) = 0x3f800000;
        *(float *)(t + 0x78) = 2.0f / fh;
        *(unsigned int *)(t + 0xbc) = g_init_const_7e7b4;
        *(unsigned int *)(t + 0xe0) = 0x3f800000;
        *(unsigned int *)(t + 0xc8) = 0x3f800000;
        *(float *)(t + 0xec) = fw;
        ymul = fw;
    }
    *(unsigned int *)(t + 0xa0) = g_init_const_7e7b8;
    *(unsigned int *)(t + 0xb4) = 0x3f800000;
    *(unsigned int *)(t + 0xa8) = g_init_const_7e7b4;
    *(unsigned int *)(t + 0xac) = 0x3f800000;
    *(float *)(t + 0x8c) = -2.0f / ymul;

    paintcanvas_ext_init_setpersp(self, 16384.0f, 512.0f, 65536.0f);
}
