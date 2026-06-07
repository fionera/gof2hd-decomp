#include "class.h"

extern char paintcanvas_g_cube_enabled;
extern int paintcanvas_g_cube_slot;
extern "C" void paintcanvas_ext_gl_a(unsigned int);
extern "C" void paintcanvas_ext_gl_bind(unsigned int, unsigned int);
extern "C" void paintcanvas_ext_gl_c(unsigned int);
extern "C" void *paintcanvas_ext_cube_restore(void *);
extern "C" void paintcanvas_ext_cube_tail(void *);

void ChangeCubeTexture(AbyssEngine::PaintCanvas *self, unsigned int idx)
{
    if (paintcanvas_g_cube_enabled != 0 && idx < *(unsigned int *)((char *)self + 0x10)) {
        char *tex = (*(char ***)((char *)self + 0x14))[idx];
        if (*(unsigned char *)(tex + 0x14) == 0) {
            return paintcanvas_ext_cube_tail(paintcanvas_ext_cube_restore(tex + 0x4));
        }
        paintcanvas_g_cube_slot = idx;
        paintcanvas_ext_gl_a(0x84c7);
        char *tex2 = (*(char ***)((char *)self + 0x14))[idx];
        paintcanvas_ext_gl_bind(0x8513, *(unsigned int *)tex2);
        return paintcanvas_ext_gl_c(0x84c0);
    }
}
