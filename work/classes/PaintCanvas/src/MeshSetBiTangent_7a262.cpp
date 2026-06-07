#include "class.h"

using AbyssEngine::AEMath::Vector;

void MeshSetBiTangent(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short vtx, const Vector &value)
{
    if (index < *(unsigned int *)((char *)self + 0x24)) {
        char *mesh = (*(char ***)((char *)self + 0x28))[index];
        if (vtx >= *(unsigned short *)(mesh + 0x2)) {
            return;
        }
        char *base = *(char **)(mesh + 0x18);
        *(Vector *)(base + vtx * 12) = value;
    }
}
