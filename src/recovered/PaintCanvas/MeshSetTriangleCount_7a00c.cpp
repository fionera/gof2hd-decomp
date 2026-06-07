#include "class.h"

void MeshSetTriangleCount(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short count)
{
    if (index < *(unsigned int *)((char *)self + 0x24)) {
        char *mesh = (*(char ***)((char *)self + 0x28))[index];
        unsigned short cap = *(unsigned short *)(mesh + 0x2a);
        if (cap < count) {
            count = cap;
        }
        *(unsigned short *)(mesh + 0x28) = (unsigned short)(count * 3);
    }
}
