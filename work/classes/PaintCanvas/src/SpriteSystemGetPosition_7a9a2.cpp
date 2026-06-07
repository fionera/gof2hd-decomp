#include "class.h"

using AbyssEngine::AEMath::Vector;

void SpriteSystemGetPosition(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub, Vector &out)
{
    if (index < *(unsigned int *)((char *)self + 0x180)) {
        unsigned short *s = (*(unsigned short ***)((char *)self + 0x184))[index];
        if (s) {
            if ((unsigned int)*s <= (unsigned int)sub) {
                return;
            }
            float *p = (float *)(*(char **)((char *)s + 0x4) + sub * 12);
            out.x = p[0];
            out.y = p[1];
            out.z = p[2];
        }
    }
}
