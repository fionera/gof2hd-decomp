#include "class.h"

extern "C" char *paintcanvas_ext_find_res(void *, unsigned int);

void MaterialResourceChangeTexture(AbyssEngine::PaintCanvas *self, unsigned short resId,
                                   void *texture, unsigned int slot)
{
    if (slot <= 7) {
        char *r = paintcanvas_ext_find_res(self, resId);
        if (r) {
            unsigned int matIdx = *(unsigned int *)(r + 0x8);
            if (matIdx + 1 != 0 && matIdx < *(unsigned int *)((char *)self + 0x174)) {
                char *mat = (*(char ***)((char *)self + 0x178))[matIdx];
                *(void **)(mat + slot * 4) = texture;
            }
        }
    }
}
