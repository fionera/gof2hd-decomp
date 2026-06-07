#include "class.h"

void SpriteSystemAddSize(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub, short delta)
{
    if (index < *(unsigned int *)((char *)self + 0x180)) {
        short *s = (*(short ***)((char *)self + 0x184))[index];
        if (s) {
            if ((unsigned short)*s <= (unsigned int)sub) {
                return;
            }
            short *vals = *(short **)((char *)s + 0x8);
            if (*(char *)((char *)s + 0xc) != 0) {
                vals[0] += delta;
                return;
            }
            vals[sub] += delta;
        }
    }
}
