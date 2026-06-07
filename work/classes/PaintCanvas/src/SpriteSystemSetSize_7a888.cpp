#include "class.h"

void SpriteSystemSetSize(AbyssEngine::PaintCanvas *self, unsigned int index,
                         unsigned int sub, unsigned short value)
{
    if (index < *(unsigned int *)((char *)self + 0x180)) {
        char *s = (*(char ***)((char *)self + 0x184))[index];
        if (s) {
            if (sub < *(unsigned short *)s) {
                unsigned short *p = *(unsigned short **)(s + 0x8);
                if (*(unsigned char *)(s + 0xc)) {
                    p[0] = value;
                } else {
                    p[sub] = value;
                }
            }
        }
    }
}
