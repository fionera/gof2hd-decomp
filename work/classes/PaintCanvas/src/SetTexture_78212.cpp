#include "class.h"

extern "C" void paintcanvas_ext_set_texture(void *);

void SetTexture(AbyssEngine::PaintCanvas *self, unsigned int, unsigned int)
{
    return paintcanvas_ext_set_texture(*(void **)((char *)self + 0x34));
}
