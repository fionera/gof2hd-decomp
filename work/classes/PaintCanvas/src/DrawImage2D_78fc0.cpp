#include "class.h"

extern "C" void paintcanvas_ext_di4_restore(unsigned int flag, void *img);
extern "C" void paintcanvas_ext_di4_settexture(void *self, unsigned int tex);
extern "C" int paintcanvas_ext_di4_getwidth(void *self);
extern "C" int paintcanvas_ext_di4_getheight(void *self);
extern "C" float paintcanvas_ext_di4_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_di4_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_di4_gldisable(unsigned int cap);
extern "C" void paintcanvas_ext_di4_meshdraw(void *eng, void *mesh);
extern "C" void paintcanvas_ext_di4_glenable(unsigned int cap);

void DrawImage2D(AbyssEngine::PaintCanvas *self, unsigned int param_1, int param_2, int param_3,
                 int param_4, int param_5, unsigned char param_6, unsigned char param_7,
                 unsigned char param_8)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x14c)) {
        return;
    }
    char *img = (*(char ***)(t + 0x150))[param_1];
    if (*(unsigned char *)(img + 0x14) != 0) {
        paintcanvas_ext_di4_restore(*(unsigned char *)(img + 0x14), img);
        img = (*(char ***)(t + 0x150))[param_1];
    }
    paintcanvas_ext_di4_settexture(self, *(unsigned int *)(img + 4));

    // horizontal base placement (param_6 low bits)
    int baseX = param_2;
    int hOff;
    if ((param_6 & 7) == 4) {
        hOff = paintcanvas_ext_di4_getwidth(self) >> 1;
    } else if ((param_6 & 7) == 2) {
        baseX = -param_2;
        hOff = paintcanvas_ext_di4_getwidth(self);
    } else {
        hOff = 0;
    }

    // horizontal flip-span (param_6 bit 3)
    int spanW = param_4;
    if (param_6 & 8) {
        spanW = paintcanvas_ext_di4_getwidth(self) - (param_4 + param_2);
    }
    float fSpanW = paintcanvas_ext_di4_signedtofloat(spanW, 0);
    char *region = *(char **)(*(char **)((*(char ***)(t + 0x150))[param_1]) + 4);
    float regW = *(float *)(region + 0xc);

    // vertical flip-span (param_6 bit 7)
    int spanH = param_5;
    if (param_6 & 0x80) {
        spanH = paintcanvas_ext_di4_getheight(self) - (param_5 + param_3);
        region = *(char **)(*(char **)((*(char ***)(t + 0x150))[param_1]) + 4);
    }
    float fSpanH = paintcanvas_ext_di4_signedtofloat(spanH, 0);
    float regH = *(float *)(region + 0x1c);

    // vertical base placement (param_6 high nibble)
    int baseY = param_3;
    int vOff;
    if ((param_6 & 0x70) == 0x40) {
        vOff = paintcanvas_ext_di4_getheight(self) >> 1;
    } else if ((param_6 & 0x70) == 0x20) {
        vOff = paintcanvas_ext_di4_getheight(self);
        baseY = -param_3;
    } else {
        vOff = 0;
    }

    // anchor offset X (param_7 low bits)
    int anchorX;
    if ((param_7 & 7) == 4) {
        if ((param_6 & 8) == 0) {
            anchorX = -(param_4 >> 1);
        } else {
            anchorX = ((param_4 + param_2) - paintcanvas_ext_di4_getwidth(self)) >> 1;
        }
    } else if ((param_7 & 7) == 2) {
        if ((param_6 & 8) == 0) {
            anchorX = -param_4;
        } else {
            anchorX = (param_4 + param_2) - paintcanvas_ext_di4_getwidth(self);
        }
    } else {
        anchorX = 0;
    }

    float scaleX = fSpanW / regW;
    float scaleY = fSpanH / regH;

    // anchor offset Y (param_7 high nibble)
    int anchorY;
    if ((param_7 & 0x70) == 0x40) {
        if ((param_6 & 0x80) == 0) {
            anchorY = -(param_5 >> 1);
        } else {
            anchorY = ((param_5 + param_3) - paintcanvas_ext_di4_getheight(self)) >> 1;
        }
    } else if ((param_7 & 0x70) == 0x20) {
        if ((param_6 & 0x80) == 0) {
            anchorY = -param_5;
        } else {
            anchorY = (param_5 + param_3) - paintcanvas_ext_di4_getheight(self);
        }
    } else {
        anchorY = 0;
    }

    float m[16];
    memset(m, 0, sizeof(m));
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
    m[3] = paintcanvas_ext_di4_signedtofloat(baseX + hOff + anchorX, 0);
    m[7] = paintcanvas_ext_di4_signedtofloat(anchorY + vOff + baseY, 0);
    m[0] = scaleX;

    if (param_8 & 1) {
        m[0] = -scaleX;
        m[3] = paintcanvas_ext_di4_signedtofloat(param_4, 0) + m[3];
    }
    m[5] = scaleY;
    if (param_8 & 2) {
        m[5] = -scaleY;
        m[7] = paintcanvas_ext_di4_signedtofloat(param_5, 0) + m[7];
    }

    paintcanvas_ext_di4_setwvm(self, m);
    paintcanvas_ext_di4_gldisable(0xb44);
    paintcanvas_ext_di4_meshdraw(*(void **)(t + 0x34),
                                 *(void **)((*(char ***)(t + 0x150))[param_1]));
    paintcanvas_ext_di4_glenable(0xb44);
}
