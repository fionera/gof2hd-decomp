#include "class.h"

#define HIDDEN __attribute__((visibility("hidden")))
extern char *paintcanvas_g_b2d_flag HIDDEN;
extern "C" void paintcanvas_ext_gl_disable(unsigned int);
extern "C" void paintcanvas_ext_gl_depthmask(unsigned int);
extern "C" void paintcanvas_ext_gl_enable(unsigned int);
extern "C" void paintcanvas_ext_gl_blendfunc(unsigned int, unsigned int);
extern "C" void paintcanvas_ext_setcolor(void *, float, float, float, float);
extern "C" void paintcanvas_ext_glenable2(void *, unsigned int, bool);
extern "C" void paintcanvas_ext_setortho(void *, void *, void *, bool);
extern "C" void paintcanvas_ext_gl_texenvi(unsigned int, unsigned int, unsigned int);
extern "C" void paintcanvas_ext_glMatrixMode(unsigned int);
extern "C" void paintcanvas_ext_gl_loadidentity();
extern "C" void paintcanvas_ext_gl_scalef(float, float, float);
extern "C" void paintcanvas_ext_gl_loadmatrix(void *);
extern "C" void paintcanvas_ext_gl_multmatrix(void *);

void Begin2d(AbyssEngine::PaintCanvas *self)
{
    *(unsigned char *)(*(char **)((char *)self + 0x34) + 0xfd) = 1;
    paintcanvas_ext_gl_disable(0xb71);
    paintcanvas_ext_gl_depthmask(0);
    paintcanvas_ext_gl_enable(0xbe2);
    paintcanvas_ext_gl_blendfunc(0x302, 0x303);
    paintcanvas_ext_setcolor(*(void **)((char *)self + 0x34), 1.0f, 1.0f, 1.0f, 1.0f);
    paintcanvas_ext_glenable2(*(void **)((char *)self + 0x34), 0xde1, true);
    if (*paintcanvas_g_b2d_flag == 0) {
        paintcanvas_ext_gl_texenvi(0x2300, 0x2200, 0x2100);
        paintcanvas_ext_glMatrixMode(0x1702);
        paintcanvas_ext_gl_loadidentity();
        paintcanvas_ext_gl_scalef(1.0f, 1.0f, 1.0f);
        paintcanvas_ext_glMatrixMode(0x1701);
        paintcanvas_ext_gl_loadmatrix((char *)self + 0x78);
        if (*(int *)((char *)self + 0x30) != 2) {
            paintcanvas_ext_gl_multmatrix((char *)self + 0xb8);
        }
        paintcanvas_ext_glMatrixMode(0x1700);
        paintcanvas_ext_gl_loadidentity();
    } else {
        paintcanvas_ext_setortho(*(void **)((char *)self + 0x34), (char *)self + 0x78,
                                 (char *)self + 0xb8, *(int *)((char *)self + 0x30) != 2);
    }
    *(int *)((char *)self + 0xc) = 0;
}
