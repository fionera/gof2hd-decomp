#include "class.h"

extern "C" void paintcanvas_ext_start_fbo(void *);

void StartDraw2FBO(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_start_fbo(*(void **)((char *)self + 0x34));
}
