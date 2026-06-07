#include "class.h"

extern "C" void paintcanvas_ext_sprite_alluv(float, float, float, float, void *);

void SpriteSystemSetAllUv(AbyssEngine::PaintCanvas *self, unsigned int index,
                          float a, float b, float c, float d)
{
    if (*(unsigned int *)((char *)self + 0x180) <= index) {
        return;
    }
    void *sprite = (*(void ***)((char *)self + 0x184))[index];
    if (sprite == 0) {
        return;
    }
    return paintcanvas_ext_sprite_alluv(a, b, c, d, sprite);
}
