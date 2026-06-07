#include "class.h"

float MeshSetPoint(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short vtx,
                   float x, float y, float z)
{
    if (index < *(unsigned int *)((char *)self + 0x24)) {
        char *mesh = (*(char ***)((char *)self + 0x28))[index];
        if (vtx < *(unsigned short *)(mesh + 0x2)) {
            float *p = (float *)(*(char **)(mesh + 0x4) + vtx * 12);
            p[0] = x;
            p[1] = y;
            p[2] = z;
            return z;
        }
    }
    return x;
}
