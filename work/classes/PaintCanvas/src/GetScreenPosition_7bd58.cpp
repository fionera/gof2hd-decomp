#include "class.h"

__attribute__((visibility("hidden"))) extern const double g_gsp2_gravscale_8bfa8;

extern "C" void paintcanvas_ext_gsp2_transformvec(void *out, void *vec);
extern "C" void paintcanvas_ext_gsp2_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_gsp2_getgrav(void *eng);
extern "C" float paintcanvas_ext_gsp2_sinf(float v);
extern "C" float paintcanvas_ext_gsp2_cosf(float v);
extern "C" void paintcanvas_ext_gsp2_memcpy(void *dst, void *src, unsigned int n);
extern "C" void paintcanvas_ext_gsp2_mtx_muleq(void *m, void *rhs);
extern "C" void paintcanvas_ext_gsp2_invtransformvec(void *outMat, void *vec);
extern "C" void paintcanvas_ext_gsp2_vec_assign(void *dst, void *src);
extern "C" int paintcanvas_ext_gsp2_getwidth(void *self);
extern "C" int paintcanvas_ext_gsp2_getheight(void *self);
extern "C" float paintcanvas_ext_gsp2_signedtofloat(int v, unsigned int mode);

void GetScreenPosition(AbyssEngine::PaintCanvas *self, void *param_1, void *param_2, char *param_3)
{
    char *t = (char *)self;

    // first transform the input world position
    char transformed[16];
    paintcanvas_ext_gsp2_transformvec(transformed, param_1);

    if (*(unsigned int *)(t + 0x170) >= *(unsigned int *)(t + 0x164)) {
        return;
    }

    char invMat[60];
    void *cam = (*(void ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
    if (*t == 0) {
        paintcanvas_ext_gsp2_invtransformvec(invMat, (char *)cam + 0xc);
        paintcanvas_ext_gsp2_vec_assign(param_3, invMat);
    } else {
        float m[16];
        char scratch[60];
        memset(m, 0, sizeof(m));
        m[0] = 1.0f; m[5] = 1.0f; m[14] = 1.0f;
        paintcanvas_ext_gsp2_matidentity(scratch, m);

        void *grav = paintcanvas_ext_gsp2_getgrav(*(void **)(t + 0x34));
        double angle = *(double *)((char *)grav + 8) * g_gsp2_gravscale_8bfa8;
        float a = (float)angle;
        int orient = *(int *)(t + 0x30);
        float rot = (orient == 1) ? a : -a;
        float s = paintcanvas_ext_gsp2_sinf(rot);
        float c = paintcanvas_ext_gsp2_cosf(rot);
        m[0] = c;
        m[5] = c;
        *(unsigned int *)&m[1] = *(unsigned int *)&s ^ 0x80000000;
        m[4] = s;

        paintcanvas_ext_gsp2_memcpy(scratch, (char *)cam + 0xc, 0x3c);
        paintcanvas_ext_gsp2_mtx_muleq(scratch, m);
        paintcanvas_ext_gsp2_invtransformvec(invMat, scratch);
        paintcanvas_ext_gsp2_vec_assign(param_3, invMat);
    }

    float z = *(float *)(param_3 + 8);
    char *cam2 = (*(char ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
    if (z > *(float *)(cam2 + 4)) {
        return;
    }
    float denomX = z * *(float *)(cam2 + 0x4c);
    if (denomX == 0.0f) {
        return;
    }
    float denomY = z * *(float *)(cam2 + 0x48);
    if (denomY == 0.0f) {
        return;
    }

    float px = *(float *)param_3;
    int w0 = paintcanvas_ext_gsp2_getwidth(self);
    int w1 = paintcanvas_ext_gsp2_getwidth(self);
    float py = *(float *)(param_3 + 4);
    double fw = (double)paintcanvas_ext_gsp2_signedtofloat(w0, 0);
    double halfW = (double)paintcanvas_ext_gsp2_signedtofloat(w1 >> 1, 0);
    *(float *)param_3 = (float)(halfW - (((double)px * 0.5) / (double)denomX) * fw);

    int h0 = paintcanvas_ext_gsp2_getheight(self);
    double fh = (double)paintcanvas_ext_gsp2_signedtofloat(h0, 0);
    int h1 = paintcanvas_ext_gsp2_getheight(self);
    double halfH = (double)paintcanvas_ext_gsp2_signedtofloat(h1 >> 1, 0);
    *(float *)(param_3 + 4) = (float)(halfH + (((double)py * 0.5) / (double)denomY) * fh);

    // remaining bound checks compute booleans only (no observable output)
    float nx = *(float *)param_3;
    if (nx >= 0.0f) {
        float ny = *(float *)(param_3 + 4);
        if (ny >= 0.0f) {
            int ww = paintcanvas_ext_gsp2_getwidth(self);
            float fww = paintcanvas_ext_gsp2_signedtofloat(ww, 0);
            if (nx < fww) {
                int hh = paintcanvas_ext_gsp2_getheight(self);
                paintcanvas_ext_gsp2_signedtofloat(hh, 0);
            }
        }
    }
}
