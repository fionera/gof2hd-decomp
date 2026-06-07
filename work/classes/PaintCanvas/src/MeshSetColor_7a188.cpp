#include "class.h"

void MeshSetColor(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                  float r, float g, float b, float a)
{
    if (index < *(unsigned int *)((char *)self + 0x24)) {
        char *mesh = (*(char ***)((char *)self + 0x28))[index];
        if ((unsigned int)sub < (unsigned int)*(unsigned short *)(mesh + 0x2)) {
            float *p = (float *)(*(char **)(mesh + 0xc) + sub * 0x10);
            p[0] = r;
            p[1] = g;
            p[2] = b;
            p[3] = a;
        }
    }
}
