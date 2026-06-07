#include "class.h"

extern "C" void paintcanvas_ext_convert_vbo(void *);

void MeshConvertToVBO(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < *(unsigned int *)((char *)self + 0x24)) {
        return paintcanvas_ext_convert_vbo(
            (*(void ***)((char *)self + 0x28))[index]);
    }
}
