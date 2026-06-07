#include "class.h"

extern "C" void paintcanvas_ext_sprite_uv(unsigned int, float, float, float, float, void *);

void SpriteSystemSetUv(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                       float a, float b, float c, float d)
{
    if (index < *(unsigned int *)((char *)self + 0x180)) {
        unsigned short *s = (*(unsigned short ***)((char *)self + 0x184))[index];
        if (s) {
            if ((unsigned int)*s <= (unsigned int)sub) {
                return;
            }
            return paintcanvas_ext_sprite_uv(sub, a, b, c, d, s);
        }
    }
}
