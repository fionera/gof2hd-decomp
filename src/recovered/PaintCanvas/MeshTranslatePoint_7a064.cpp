#include "class.h"

void MeshTranslatePoint(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                        float x, float y, float z)
{
    if (index < *(unsigned int *)((char *)self + 0x24)) {
        char *mesh = (*(char ***)((char *)self + 0x28))[index];
        if ((unsigned int)*(unsigned short *)(mesh + 0x2) <= (unsigned int)sub) {
            return;
        }
        float *p = (float *)(*(char **)(mesh + 0x4) + sub * 12);
        p[0] = p[0] + x;
        p[1] = p[1] + y;
        p[2] = p[2] + z;
    }
}
