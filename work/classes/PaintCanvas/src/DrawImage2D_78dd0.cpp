#include "class.h"

extern "C" void paintcanvas_ext_di3_restore(unsigned int flag, void *img);
extern "C" int paintcanvas_ext_di3_getwidth(void *self);
extern "C" int paintcanvas_ext_di3_getheight(void *self);
extern "C" float paintcanvas_ext_di3_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_di3_settexture(void *self, unsigned int tex);
extern "C" void paintcanvas_ext_di3_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_di3_meshdraw(void *eng, void *mesh);

void DrawImage2D(AbyssEngine::PaintCanvas *self, unsigned int param_1, int param_2, int param_3,
                 unsigned char param_4, unsigned char param_5)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x14c)) {
        return;
    }
    char *img = (*(char ***)(t + 0x150))[param_1];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_di3_restore(*(unsigned char *)(img + 0x14), img);
    }

    // Horizontal placement offset (param_5 low bits) + vertical (param_5 high nibble).
    int hOff;
    if ((param_5 & 7) == 4) {
        hOff = paintcanvas_ext_di3_getwidth(self) >> 1;
    } else if ((param_5 & 7) == 2) {
        hOff = paintcanvas_ext_di3_getwidth(self);
        param_3 = -param_3;
    } else {
        hOff = 0;
    }

    int vOff;
    unsigned int yShift = param_4;
    if ((param_5 & 0x70) == 0x40) {
        vOff = paintcanvas_ext_di3_getheight(self) >> 1;
    } else if ((param_5 & 0x70) == 0x20) {
        vOff = paintcanvas_ext_di3_getheight(self);
        yShift = (unsigned int)(-(int)yShift);
    } else {
        vOff = 0;
    }

    // Region-based offsets from the image's source region (param_4 used as a second align flag).
    int rx;
    char *region = *(char **)(img + 4);
    if ((param_4 & 7) == 4) {
        double w = (double)paintcanvas_ext_di3_signedtofloat((int)*(float *)(region + 0xc), 0);
        rx = (int)(long long)(w * -0.5);
    } else if ((param_4 & 7) == 2) {
        rx = -(int)*(float *)(region + 0xc);
    } else {
        rx = 0;
    }

    int ry;
    char *region2 = *(char **)((*(char ***)(t + 0x150))[param_1] + 4);
    if ((param_4 & 0x70) == 0x20) {
        ry = -(int)*(float *)(region2 + 0x1c);
    } else if ((param_4 & 0x70) == 0x40) {
        double h = (double)paintcanvas_ext_di3_signedtofloat((int)*(float *)(region2 + 0x1c), 0);
        ry = (int)(long long)(h * -0.5);
    } else {
        ry = 0;
    }

    char *img2 = (*(char ***)(t + 0x150))[param_1];
    paintcanvas_ext_di3_settexture(self, *(unsigned int *)(img2 + 4));

    float fx = paintcanvas_ext_di3_signedtofloat(hOff + param_3 + rx, 0);
    float fy = paintcanvas_ext_di3_signedtofloat(vOff + (int)yShift + ry, 0);

    float m[16];
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
    m[3] = fx;
    m[7] = fy;

    paintcanvas_ext_di3_setwvm(self, m);
    paintcanvas_ext_di3_meshdraw(*(void **)(t + 0x34),
                                 *(void **)((*(char ***)(t + 0x150))[param_1]));
}
