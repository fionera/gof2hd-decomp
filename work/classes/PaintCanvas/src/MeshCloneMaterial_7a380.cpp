#include "class.h"

extern "C" void *paintcanvas_ext_alloc(unsigned int);
extern "C" void paintcanvas_ext_material_clone(void *, void *);
extern "C" void paintcanvas_ext_material_add(void *, void *);

void MeshCloneMaterial(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned int *out)
{
    int result;
    if (index < *(unsigned int *)((char *)self + 0x24)) {
        char *obj = (char *)paintcanvas_ext_alloc(0x74);
        char *mesh = (*(char ***)((char *)self + 0x28))[index];
        paintcanvas_ext_material_clone(obj, *(void **)(mesh + 0x30));
        paintcanvas_ext_material_add(obj, (char *)self + 0x174);
        result = (int)*(unsigned int *)((char *)self + 0x174) - 1;
    } else {
        result = -1;
    }
    *out = result;
}
