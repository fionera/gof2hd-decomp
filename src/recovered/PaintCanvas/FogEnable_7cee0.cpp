#include "class.h"

#define HIDDEN __attribute__((visibility("hidden")))
extern char *paintcanvas_g_fog_flag HIDDEN;
extern char *paintcanvas_g_fog_ptr HIDDEN;
extern "C" void paintcanvas_ext_gl_disable(unsigned int);
extern "C" void paintcanvas_ext_gl_enable(unsigned int);

void FogEnable(AbyssEngine::PaintCanvas *self, int mode, int enable)
{
    *(int *)((char *)self + 0x1f4) = enable;
    if (enable == 0) {
        if (*paintcanvas_g_fog_flag != 0) {
            *paintcanvas_g_fog_ptr = (char)mode;
        } else if (mode == 0) {
            paintcanvas_ext_gl_disable(0xb60);
        } else {
            paintcanvas_ext_gl_enable(0xb60);
        }
        mode = 0;
    }
    *(char *)((char *)self + 0x1f1) = (char)mode;
}
