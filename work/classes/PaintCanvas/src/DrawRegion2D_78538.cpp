#include "class.h"

__attribute__((visibility("hidden"))) extern const unsigned int g_dr3_const_88808;

extern "C" void paintcanvas_ext_dr3_settexture(void *self, unsigned int tex);
extern "C" float paintcanvas_ext_dr3_signedtofloat(int v, unsigned int mode);
extern "C" float paintcanvas_ext_dr3_unsignedtofloat(unsigned int v, unsigned int mode);
extern "C" float paintcanvas_ext_dr3_sinf(float v);
extern "C" float paintcanvas_ext_dr3_cosf(float v);
extern "C" void paintcanvas_ext_dr3_mtx_mul(void *out, void *m);
extern "C" void paintcanvas_ext_dr3_mtx_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_dr3_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_dr3_gldisable(unsigned int cap);
extern "C" void paintcanvas_ext_dr3_meshdraw(void *eng, void *mesh);
extern "C" void paintcanvas_ext_dr3_glenable(unsigned int cap);

void DrawRegion2D(AbyssEngine::PaintCanvas *self, unsigned int param_1, int param_2, int param_3,
                  int param_4, int param_5, float param_6, int param_7, int param_8, int param_9,
                  int param_10)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x14c)) {
        return;
    }
    char *img = (*(char ***)(t + 0x150))[param_1];
    paintcanvas_ext_dr3_settexture(self, *(unsigned int *)(img + 4));
    char *mesh = (*(char ***)(t + 0x150))[param_1];
    char *meshObj = *(char **)mesh;

    *(unsigned char *)(mesh + 0x14) = 1;

    // vertex positions (8 floats) at mesh[0]+4 -> +0xc
    float fw = paintcanvas_ext_dr3_signedtofloat(param_4, 0);
    float fh = paintcanvas_ext_dr3_signedtofloat(param_5, 0);
    float *verts = (float *)(*(char **)(meshObj + 4));
    verts[0] = 0.0f; verts[1] = 0.0f; verts[2] = 0.0f;
    verts[3] = fw;       // +0xc
    verts[4] = 0.0f;     // +0x10
    verts[5] = 0.0f;
    verts[6] = fw;
    verts[7] = fh;       // +0x1c
    verts[8] = 0.0f;
    verts[9] = 0.0f;
    verts[10] = fh;      // +0x28

    // UV computations from the source region metadata stored in mesh fields 0x8..0xe
    float u0 = paintcanvas_ext_dr3_unsignedtofloat(*(unsigned short *)(mesh + 0xa), 0);
    float w  = paintcanvas_ext_dr3_unsignedtofloat(*(unsigned short *)(mesh + 8), 0);
    float h  = paintcanvas_ext_dr3_unsignedtofloat(*(unsigned short *)(mesh + 0xc), 0);
    float v0 = paintcanvas_ext_dr3_unsignedtofloat(*(unsigned short *)(mesh + 0xe), 0);
    float a2 = paintcanvas_ext_dr3_signedtofloat(param_2, 0);
    float a3 = paintcanvas_ext_dr3_signedtofloat(param_3, 0);
    float a2s = paintcanvas_ext_dr3_signedtofloat((short)param_2, 0);

    float *uv = (float *)(*(char **)(meshObj + 8));
    float invW = 1.0f / w;
    float invH = 1.0f / u0;
    float ulo = (a2 + h) * invW;
    float vlo = (a3 + v0) * invH;
    float uhi = (fw + h + a2s) * invW;
    float vhi = (fh + v0 + a3) * invH;
    uv[0] = ulo; uv[1] = vlo;
    uv[2] = uhi; uv[3] = vlo;
    uv[4] = uhi; uv[5] = vhi;
    uv[6] = ulo; uv[7] = vhi;

    // blend-mode field at meshObj+0x2c
    unsigned int *blend = *(unsigned int **)(meshObj + 0x2c);
    blend[0] = 0x20000;
    blend[1] = 1;
    blend[2] = g_dr3_const_88808;

    // pivot translation matrix local_b0
    float pivotM[16];
    memset(pivotM, 0, sizeof(pivotM));
    pivotM[0] = 1.0f; pivotM[5] = 1.0f; pivotM[10] = 1.0f; pivotM[15] = 1.0f;
    pivotM[3] = paintcanvas_ext_dr3_signedtofloat(-param_8, 0);
    pivotM[7] = paintcanvas_ext_dr3_signedtofloat(-param_9, 0);

    // inverse-pivot translation local_70
    float invPivotM[16];
    memset(invPivotM, 0, sizeof(invPivotM));
    invPivotM[0] = 1.0f; invPivotM[5] = 1.0f; invPivotM[10] = 1.0f; invPivotM[15] = 1.0f;
    invPivotM[3] = paintcanvas_ext_dr3_signedtofloat(param_10 + param_8, 0);
    invPivotM[7] = paintcanvas_ext_dr3_signedtofloat(param_7 + param_9, 0);

    // rotation matrix local_f0 (angle = invPivotM[7])
    float rotAng = invPivotM[7];
    float s = paintcanvas_ext_dr3_sinf(rotAng);
    float c = paintcanvas_ext_dr3_cosf(rotAng);
    float rotM[16];
    memset(rotM, 0, sizeof(rotM));
    rotM[0] = c;
    rotM[5] = c;
    rotM[10] = 1.0f;
    rotM[15] = 1.0f;
    *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
    rotM[4] = s;

    float local130[16];
    float scratch[16];
    paintcanvas_ext_dr3_mtx_mul(scratch, rotM);
    paintcanvas_ext_dr3_mtx_assign(local130, scratch);
    paintcanvas_ext_dr3_mtx_mul(scratch, pivotM);
    paintcanvas_ext_dr3_mtx_assign(local130, scratch);

    paintcanvas_ext_dr3_setwvm(self, local130);
    paintcanvas_ext_dr3_gldisable(0xb44);
    paintcanvas_ext_dr3_meshdraw(*(void **)(t + 0x34),
                                 *(void **)((*(char ***)(t + 0x150))[param_1]));
    paintcanvas_ext_dr3_glenable(0xb44);
}
