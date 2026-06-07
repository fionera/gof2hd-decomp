#include "class.h"

extern "C" void paintcanvas_ext_release_sprite_res(void *, void *);

void ReleaseSpriteSystemResource(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < *(unsigned int *)((char *)self + 0x180)) {
        void *ctx = *(void **)((char *)self + 0x34);
        char *arr = *(char **)((char *)self + 0x184);
        return paintcanvas_ext_release_sprite_res(ctx, arr + index * 4);
    }
}
