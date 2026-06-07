#include "class.h"

extern "C" void paintcanvas_ext_fbo_a(void *);
extern "C" void paintcanvas_ext_fbo_b(void *, int);
extern "C" void paintcanvas_ext_fbo_c(void *);
extern "C" void paintcanvas_ext_fbo_d(void *);

void StopDraw2FBO(AbyssEngine::PaintCanvas *self)
{
    paintcanvas_ext_fbo_a(*(void **)((char *)self + 0x34));
    paintcanvas_ext_fbo_b(self, 0);
    paintcanvas_ext_fbo_c(*(void **)((char *)self + 0x34));
    return paintcanvas_ext_fbo_d(*(void **)((char *)self + 0x34));
}
