#include "class.h"

extern "C" void paintcanvas_ext_remove_meshid(void *, void *);

void TransformRemoveMeshId(AbyssEngine::PaintCanvas *self, unsigned int transformIndex, unsigned int meshIndex)
{
    if (transformIndex < *(unsigned int *)((char *)self + 0x158) &&
        meshIndex < *(unsigned int *)((char *)self + 0x24)) {
        void *mesh = (*(void ***)((char *)self + 0x28))[meshIndex];
        char *t = (*(char ***)((char *)self + 0x15c))[transformIndex];
        return paintcanvas_ext_remove_meshid(mesh, t + 0x3c);
    }
}
