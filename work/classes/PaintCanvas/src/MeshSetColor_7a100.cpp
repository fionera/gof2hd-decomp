#include "class.h"

void MeshSetColor(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                  unsigned int color)
{
    if (index < *(unsigned int *)((char *)self + 0x24)) {
        char *mesh = (*(char ***)((char *)self + 0x28))[index];
        if ((unsigned int)sub < (unsigned int)*(unsigned short *)(mesh + 0x2)) {
            float *p = (float *)(*(char **)(mesh + 0xc) + sub * 0x10);
            p[0] = (float)((double)(color >> 24) / 255.0);
            p[1] = (float)((double)((color >> 16) & 0xff) / 255.0);
            p[2] = (float)((double)((color >> 8) & 0xff) / 255.0);
            p[3] = (float)((double)(color & 0xff) / 255.0);
        }
    }
}
