#include "class.h"

extern "C" void paintcanvas_ext_sprite_allsize(unsigned int, void *);

void SpriteSystemSetAllSize(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned int size)
{
    if (index < *(unsigned int *)((char *)self + 0x180)) {
        void *sprite = (*(void ***)((char *)self + 0x184))[index];
        if (sprite) {
            return paintcanvas_ext_sprite_allsize(size, sprite);
        }
    }
}
