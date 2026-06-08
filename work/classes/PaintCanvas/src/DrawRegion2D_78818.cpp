#include "class.h"

extern "C" void paintcanvas_ext_dr2_restore(unsigned int flag, void *img);
extern "C" void paintcanvas_ext_dr2_settexture(void *self, unsigned int tex);
extern "C" float paintcanvas_ext_dr2_signedtofloat(int v, unsigned int mode);
extern "C" float paintcanvas_ext_dr2_sinf(float v);
extern "C" float paintcanvas_ext_dr2_cosf(float v);
extern "C" void paintcanvas_ext_dr2_setscaling(void *out, float x, float y, float z);
extern "C" void paintcanvas_ext_dr2_mtx_mul(void *out, void *a, void *b);
extern "C" void paintcanvas_ext_dr2_mtx_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_dr2_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_dr2_gldisable(unsigned int cap);
extern "C" void paintcanvas_ext_dr2_meshdraw(void *eng, void *mesh);
extern "C" void paintcanvas_ext_dr2_glenable(unsigned int cap);

void DrawRegion2D(AbyssEngine::PaintCanvas *self, unsigned int param_1, float param_2, int param_3,
                  int param_4, int param_5, int param_6, float param_7, float param_8)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x14c)) {
        return;
    }
    char *img = (*(char ***)(t + 0x150))[param_1];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_dr2_restore(*(unsigned char *)(img + 0x14), img);
        img = (*(char ***)(t + 0x150))[param_1];
    }
    paintcanvas_ext_dr2_settexture(self, *(unsigned int *)(img + 4));

    // param_3 = rotation steps; param_4/5 = pivot; param_6/param_7? = translation; param_7/8 = scale
    float rot = paintcanvas_ext_dr2_signedtofloat(param_3, 0);
    float px = paintcanvas_ext_dr2_signedtofloat(param_4, 0);
    float py = paintcanvas_ext_dr2_signedtofloat(param_5, 0);
    float tx = paintcanvas_ext_dr2_signedtofloat(param_6, 0);
    float negpx = paintcanvas_ext_dr2_signedtofloat(-param_4, 0);
    float negpy = paintcanvas_ext_dr2_signedtofloat(-param_5, 0);

    // translation matrix local_78 (identity + translation in column 3)
    float transM[16];
    memset(transM, 0, sizeof(transM));
    transM[0] = 1.0f; transM[5] = 1.0f; transM[10] = 1.0f; transM[15] = 1.0f;
    transM[3] = py + rot * param_7;
    transM[7] = tx + px * param_8;

    // pivot matrix local_178
    float pivotM[16];
    memset(pivotM, 0, sizeof(pivotM));
    pivotM[0] = 1.0f; pivotM[5] = 1.0f; pivotM[10] = 1.0f; pivotM[15] = 1.0f;
    pivotM[3] = negpx * param_7;
    pivotM[7] = negpy * param_8;

    // rotation matrix local_f8
    float s = paintcanvas_ext_dr2_sinf(negpx * param_7);
    float c = paintcanvas_ext_dr2_cosf(negpx * param_7);
    float rotM[16];
    memset(rotM, 0, sizeof(rotM));
    rotM[0] = c;
    rotM[5] = c;
    rotM[10] = 1.0f;
    rotM[15] = 1.0f;
    *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
    rotM[4] = s;

    float scaleBuf[16];
    paintcanvas_ext_dr2_setscaling(scaleBuf, param_7, param_8, 1.0f);

    // build the composed matrix
    float local138[16];
    paintcanvas_ext_dr2_mtx_mul(scaleBuf, rotM, transM);
    paintcanvas_ext_dr2_mtx_assign(local138, scaleBuf);
    float scratch[16];
    paintcanvas_ext_dr2_mtx_mul(scratch, pivotM, local138);
    paintcanvas_ext_dr2_mtx_mul(scaleBuf, scratch, pivotM);
    paintcanvas_ext_dr2_mtx_assign(local138, scaleBuf);

    paintcanvas_ext_dr2_setwvm(self, local138);
    paintcanvas_ext_dr2_gldisable(0xb44);
    paintcanvas_ext_dr2_meshdraw(*(void **)(t + 0x34),
                                 *(void **)((*(char ***)(t + 0x150))[param_1]));
    paintcanvas_ext_dr2_glenable(0xb44);
}
