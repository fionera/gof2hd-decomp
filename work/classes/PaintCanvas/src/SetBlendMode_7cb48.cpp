#include "class.h"

__attribute__((visibility("hidden"))) extern char *const g_sbm_flag_8cb62;
__attribute__((visibility("hidden"))) extern const unsigned int g_sbm_const_8ce34;

extern "C" void paintcanvas_ext_sbm_lightenable(void *eng, int on);
extern "C" void paintcanvas_ext_sbm_lightsetlight(void *eng, int v);
extern "C" void paintcanvas_ext_sbm_glenablecap(void *eng, unsigned int v, int on);
extern "C" void paintcanvas_ext_sbm_glTexEnvi(unsigned int a, unsigned int b, unsigned int c);
extern "C" void paintcanvas_ext_sbm_glEnable(unsigned int cap);
extern "C" void paintcanvas_ext_sbm_glDisable(unsigned int cap);
extern "C" void paintcanvas_ext_sbm_glBlendFunc(unsigned int a, unsigned int b);
extern "C" void paintcanvas_ext_sbm_glDepthMask(int v);
extern "C" void paintcanvas_ext_sbm_glAlphaFunc(unsigned int a, float ref);
extern "C" void paintcanvas_ext_sbm_setalpha(void *eng, unsigned int v, int on);
extern "C" void paintcanvas_ext_sbm_setlight(int on);
extern "C" void paintcanvas_ext_sbm_texcombine(unsigned int a, unsigned int b, unsigned int c);

void SetBlendMode(AbyssEngine::PaintCanvas *self, int param_2)
{
    char *t = (char *)self;
    paintcanvas_ext_sbm_lightenable(*(void **)(t + 0x34), 0);

    char *flag = g_sbm_flag_8cb62;
    if (*flag != 0) {
        paintcanvas_ext_sbm_glenablecap(*(void **)(t + 0x34), g_sbm_const_8ce34, 0);
    } else {
        paintcanvas_ext_sbm_glTexEnvi(0x2300, 0x2200, 0x2100);
    }

    switch (param_2) {
    case 1:
    case 5:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 2:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(1, 1);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 3:
        paintcanvas_ext_sbm_glDisable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(1, 1);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 4:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0, 0x301);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 6:
        paintcanvas_ext_sbm_lightenable(*(void **)(t + 0x34), 1);
        paintcanvas_ext_sbm_lightsetlight(*(void **)(t + 0x34), 0x4000);
        /* fallthrough */
    case 0:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glDisable(0xbe2);
        paintcanvas_ext_sbm_setlight(1);
        return;
    case 7:
        paintcanvas_ext_sbm_lightenable(*(void **)(t + 0x34), 1);
        paintcanvas_ext_sbm_lightsetlight(*(void **)(t + 0x34), 0x4000);
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(1, 1);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 8:
        paintcanvas_ext_sbm_lightenable(*(void **)(t + 0x34), 1);
        paintcanvas_ext_sbm_lightsetlight(*(void **)(t + 0x34), 0x4000);
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_setlight(0);
        return;
    case 9:
        paintcanvas_ext_sbm_lightenable(*(void **)(t + 0x34), 1);
        paintcanvas_ext_sbm_lightsetlight(*(void **)(t + 0x34), 0x4000);
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_setlight(1);
        return;
    case 10:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glDisable(0xbe2);
        paintcanvas_ext_sbm_glDepthMask(1);
        paintcanvas_ext_sbm_setalpha(*(void **)(t + 0x34), 0x1000000, 1);
        if (*flag == 0) {
            paintcanvas_ext_sbm_glAlphaFunc(0x204, 0.5f);
        }
        return;
    case 0x15:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_glDepthMask(0);
        if (*flag != 0) {
            paintcanvas_ext_sbm_setalpha(*(void **)(t + 0x34), g_sbm_const_8ce34, 1);
            return;
        }
        paintcanvas_ext_sbm_texcombine(0x2300, 0x2200, 0x8570);
        paintcanvas_ext_sbm_texcombine(0x2300, 0x8571, 0x104);
        paintcanvas_ext_sbm_texcombine(0x2300, 0x8572, 0x1e01);
        paintcanvas_ext_sbm_texcombine(0x2300, 0x8580, 0x1702);
        paintcanvas_ext_sbm_texcombine(0x2300, 0x8590, 0x300);
        paintcanvas_ext_sbm_texcombine(0x2300, 0x8581, 0x8577);
        paintcanvas_ext_sbm_texcombine(0x2300, 0x8591, 0x300);
        return;
    case 0x16:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_glDepthMask(0);
        if (*flag == 0) {
            paintcanvas_ext_sbm_texcombine(0x2300, 0x2200, 0x8570);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8571, 0x8575);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8572, 0x1e01);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8580, 0x1702);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8590, 0x300);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8588, 0x1702);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8598, 0x302);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8581, 0x8577);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8591, 0x300);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8582, 0x8577);
            paintcanvas_ext_sbm_texcombine(0x2300, 0x8592, 0x302);
        }
        return;
    case 0x25:
        paintcanvas_ext_sbm_glEnable(0xb44);
        paintcanvas_ext_sbm_glEnable(0xbe2);
        paintcanvas_ext_sbm_glBlendFunc(0x302, 0x303);
        paintcanvas_ext_sbm_setlight(1);
        return;
    default:
        return;
    }
}
