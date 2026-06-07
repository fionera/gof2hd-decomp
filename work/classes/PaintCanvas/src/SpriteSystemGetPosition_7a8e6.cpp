#include "class.h"

using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

void SpriteSystemGetPosition(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                             const Matrix &m, Vector &out)
{
    if (index < *(unsigned int *)((char *)self + 0x180)) {
        char *s = (*(char ***)((char *)self + 0x184))[index];
        if (s) {
            if ((unsigned int)*(unsigned short *)s <= (unsigned int)sub) {
                return;
            }
            const float *mm = (const float *)&m;
            float *p = (float *)(*(char **)(s + 0x4) + sub * 12);
            float p0 = p[0];
            float p1 = p[1];
            float p2 = p[2];
            float half = (float)(*(short *)s >> 1);
            float x = mm[0] * p0 + mm[1] * p1 + mm[2] * p2 + mm[3];
            float y = mm[4] * p0 + mm[5] * p1 + mm[6] * p2 + mm[7];
            float z = mm[8] * p0 + mm[9] * p1 + mm[10] * p2 + mm[11];
            out.x = x - half;
            out.y = y + half;
            out.z = z;
        }
    }
}
