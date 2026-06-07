#include "class.h"

void SpriteSystemAddPosition(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                             float x, float y, float z)
{
    if (index < *(unsigned int *)((char *)self + 0x180)) {
        char *s = (*(char ***)((char *)self + 0x184))[index];
        if (s) {
            if ((unsigned int)*(unsigned short *)s <= (unsigned int)sub) {
                return;
            }
            float *p = (float *)(*(char **)(s + 0x4) + sub * 12);
            p[0] = p[0] + x;
            p[1] = p[1] + y;
            p[2] = p[2] + z;
        }
    }
}
