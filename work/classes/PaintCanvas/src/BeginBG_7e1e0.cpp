#include "class.h"

#define HIDDEN __attribute__((visibility("hidden")))

extern char paintcanvas_g_bg_a;
extern char paintcanvas_g_bg_b;
extern "C" void paintcanvas_ext_glMatrixMode(unsigned int);
extern "C" void paintcanvas_ext_gl_enable(unsigned int);
extern "C" void paintcanvas_ext_gl_depthmask(unsigned int);
extern "C" void paintcanvas_ext_gl_color(void *, float, float, float, float);
extern "C" void paintcanvas_ext_matgl_load(void *, void *);
extern "C" void paintcanvas_ext_gl_loadidentity();
extern "C" void paintcanvas_ext_gl_ortho_persp(float, float, float);
extern "C" void paintcanvas_ext_gl_loadmatrix(void *);
extern "C" void paintcanvas_ext_gl_done();

void BeginBG(AbyssEngine::PaintCanvas *self)
{
    *(unsigned char *)(*(char **)((char *)self + 0x34) + 0xfd) = 0;
    paintcanvas_ext_gl_enable(0xb71);
    paintcanvas_ext_gl_depthmask(0);
    paintcanvas_ext_gl_enable(0xbe2);
    paintcanvas_ext_gl_color(*(void **)((char *)self + 0x34), 1.0f, 1.0f, 1.0f, 1.0f);
    *(unsigned char *)((char *)self + 0x1f0) = paintcanvas_g_bg_a;
    char flag = paintcanvas_g_bg_b;
    paintcanvas_g_bg_a = 0;
    if (flag != 0) {
        return paintcanvas_ext_matgl_load(*(void **)((char *)self + 0x34), (char *)self + 0x38);
    }
    paintcanvas_ext_glMatrixMode(0x1702);
    paintcanvas_ext_gl_loadidentity();
    paintcanvas_ext_gl_ortho_persp(1.0f, 0.3f, 1.0f);
    paintcanvas_ext_glMatrixMode(0x1701);
    paintcanvas_ext_gl_loadmatrix((char *)self + 0x38);
    paintcanvas_ext_glMatrixMode(0x1700);
    return paintcanvas_ext_gl_done();
}
