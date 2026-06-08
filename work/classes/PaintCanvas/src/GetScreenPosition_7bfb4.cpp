#include "class.h"

extern "C" void paintcanvas_ext_gsp_vec_assign(void *dst, void *src);
extern "C" int paintcanvas_ext_gsp_getwidth(void *self);
extern "C" int paintcanvas_ext_gsp_getheight(void *self);
extern "C" float paintcanvas_ext_gsp_signedtofloat(int v, unsigned int mode);

void GetScreenPosition(AbyssEngine::PaintCanvas *self, char *param_1, char *param_2)
{
    char *t = (char *)self;
    if (*(unsigned int *)(t + 0x170) >= *(unsigned int *)(t + 0x164)) {
        return;
    }

    // copy {m[0xc], m[0x1c], m[0x2c]} into the output vector
    float src[3];
    src[0] = *(float *)(param_1 + 0xc);
    src[1] = *(float *)(param_1 + 0x1c);
    src[2] = *(float *)(param_1 + 0x2c);
    paintcanvas_ext_gsp_vec_assign(param_2, src);

    char *cam = (*(char ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
    float z = *(float *)(param_2 + 8);
    float denomX = *(float *)(cam + 0x4c) * z;
    if (denomX == 0.0f) {
        return;
    }
    float denomY = z * *(float *)(cam + 0x48);
    if (denomY == 0.0f) {
        return;
    }

    float px = *(float *)param_2;
    int w0 = paintcanvas_ext_gsp_getwidth(self);
    int w1 = paintcanvas_ext_gsp_getwidth(self);
    double fw = (double)paintcanvas_ext_gsp_signedtofloat(w0, 0);
    double termY = ((double)*(float *)(param_2 + 4) * 0.5) / (double)denomY;
    double halfW = (double)paintcanvas_ext_gsp_signedtofloat(w1 >> 1, 0);
    *(float *)param_2 = (float)(halfW - (((double)px * 0.5) / (double)denomX) * fw);

    int h0 = paintcanvas_ext_gsp_getheight(self);
    double fh = (double)paintcanvas_ext_gsp_signedtofloat(h0, 0);
    int h1 = paintcanvas_ext_gsp_getheight(self);
    double halfH = (double)paintcanvas_ext_gsp_signedtofloat(h1 >> 1, 0);
    *(float *)(param_2 + 4) = (float)(halfH + termY * fh);

    // remaining clamp/visibility checks have no observable side-effects on output
    // (they compute booleans only); reproduce the visible reads to keep behavior.
    char *cam2 = (*(char ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
    if (*(float *)(param_2 + 8) <= *(float *)(cam2 + 4)) {
        float fy = *(float *)(param_2 + 4);
        if (fy >= 0.0f) {
            float fx = *(float *)param_2;
            if (fx >= 0.0f) {
                int ww = paintcanvas_ext_gsp_getwidth(self);
                float fww = paintcanvas_ext_gsp_signedtofloat(ww, 0);
                if (fx < fww) {
                    int hh = paintcanvas_ext_gsp_getheight(self);
                    paintcanvas_ext_gsp_signedtofloat(hh, 0);
                }
            }
        }
    }
}
