#include "class.h"

float SpriteSystemSetPosition(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                              float x, float y, float z)
{
    if (index < *(unsigned int *)((char *)self + 0x180)) {
        char *s = (*(char ***)((char *)self + 0x184))[index];
        if (s) {
            if (sub < *(unsigned short *)s) {
                float *p = (float *)(*(char **)(s + 0x4) + sub * 12);
                p[0] = x;
                p[1] = y;
                p[2] = z;
                return z;
            }
        }
    }
    return x;
}
