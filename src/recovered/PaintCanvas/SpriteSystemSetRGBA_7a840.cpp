#include "class.h"

extern "C" void paintcanvas_ext_sprite_rgba(unsigned int, float, float, float, float, void *);

void SpriteSystemSetRGBA(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned short sub,
                         float a, float b, float c, float d)
{
    if (index < *(unsigned int *)((char *)self + 0x180)) {
        unsigned short *s = (*(unsigned short ***)((char *)self + 0x184))[index];
        if (s) {
            if ((unsigned int)*s <= (unsigned int)sub) {
                return;
            }
            return paintcanvas_ext_sprite_rgba(sub, a, b, c, d, s);
        }
    }
}
