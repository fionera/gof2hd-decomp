#include "class.h"

extern "C" void paintcanvas_ext_ec_glEnable(unsigned int cap);
extern "C" int paintcanvas_ext_ec_getHeight(void *self);
extern "C" int paintcanvas_ext_ec_getWidth(void *self);
extern "C" void paintcanvas_ext_ec_glScissor(int x, int y, int w, int h);

void EnableClip(AbyssEngine::PaintCanvas *self, int param_1, int param_2, int param_3, int param_4)
{
    paintcanvas_ext_ec_glEnable(0xc11);
    int sx, sy, sw, sh;
    switch (*(unsigned int *)((char *)self + 0x30)) {
    case 0:
        sx = param_1;
        sy = param_2;
        sw = param_3;
        sh = param_4;
        break;
    case 1: {
        int h = paintcanvas_ext_ec_getHeight(self);
        int w = paintcanvas_ext_ec_getWidth(self);
        sx = h - (param_2 + param_4);
        sy = w - (param_1 + param_3);
        sw = param_4;
        sh = param_3;
        break;
    }
    case 2: {
        int h = paintcanvas_ext_ec_getHeight(self);
        sx = h - (param_2 + param_4);
        sy = param_1;
        sw = param_3;
        sh = param_4;
        break;
    }
    case 3: {
        int w = paintcanvas_ext_ec_getWidth(self);
        sx = w - (param_1 + param_3);
        sy = param_2;
        sw = param_3;
        sh = param_4;
        break;
    }
    default:
        sx = param_1;
        sy = param_2;
        sw = param_3;
        sh = param_4;
        break;
    }
    paintcanvas_ext_ec_glScissor(sx, sy, sw, sh);
}
