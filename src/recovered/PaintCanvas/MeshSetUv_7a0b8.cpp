#include "class.h"

extern char paintcanvas_g_flipv;

void MeshSetUv(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
               float u, float v)
{
    if (index < *(unsigned int *)((char *)self + 0x24)) {
        char *mesh = (*(char ***)((char *)self + 0x28))[index];
        if ((unsigned int)sub < (unsigned int)*(unsigned short *)(mesh + 0x2)) {
            float *p = (float *)(*(char **)(mesh + 0x8) + sub * 8);
            p[0] = u;
            if (paintcanvas_g_flipv != 0) {
                p[1] = 1.0f - v;
                return;
            }
            p[1] = v;
        }
    }
}
