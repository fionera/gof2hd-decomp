#include "class.h"

void MeshSetTriangle(AbyssEngine::PaintCanvas *self, unsigned int meshIndex, unsigned short tri,
                     unsigned short v0, unsigned short v1, unsigned short v2)
{
    if (meshIndex < *(unsigned int *)((char *)self + 0x24)) {
        char *mesh = (*(char ***)((char *)self + 0x28))[meshIndex];
        unsigned int t3 = tri * 3;
        if (*(unsigned short *)(mesh + 0x28) <= t3) {
            return;
        }
        unsigned short *buf = *(unsigned short **)(mesh + 0x2c);
        buf[t3] = v0;
        buf[t3 + 1] = v1;
        buf[t3 + 2] = v2;
    }
}
