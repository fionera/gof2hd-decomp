#include "class.h"

extern "C" void paintcanvas_ext_draw_mesh(void *, void *);

void DrawMesh(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < *(unsigned int *)((char *)self + 0x24)) {
        void *mesh = (*(void ***)((char *)self + 0x28))[index];
        return paintcanvas_ext_draw_mesh(*(void **)((char *)self + 0x34), mesh);
    }
}
