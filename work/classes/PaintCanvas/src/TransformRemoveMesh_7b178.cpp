#include "class.h"

extern "C" char *paintcanvas_ext_find_mesh(void *, void *);
extern "C" void paintcanvas_ext_remove_mesh(void *, unsigned int, int);

void TransformRemoveMesh(AbyssEngine::PaintCanvas *self, unsigned int transformIndex, void *mesh)
{
    if (transformIndex >= *(unsigned int *)((char *)self + 0x158)) {
        return;
    }
    char *x = paintcanvas_ext_find_mesh(self, mesh);
    if (x) {
        return paintcanvas_ext_remove_mesh(self, transformIndex, *(int *)(x + 0x8));
    }
}
