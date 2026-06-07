#include "class.h"

extern "C" void *paintcanvas_ext_alloc(unsigned int);
extern "C" void *paintcanvas_ext_material_ctor(void *);
extern "C" void paintcanvas_ext_material_add(void *, void *);

void MaterialCreate(AbyssEngine::PaintCanvas *self, unsigned int *out, void *p2, void *p3)
{
    char *obj = (char *)paintcanvas_ext_alloc(0x74);
    paintcanvas_ext_material_ctor(obj);
    *(void **)(obj + 0x0) = p3;
    *(void **)(obj + 0x20) = p2;
    paintcanvas_ext_material_add(obj, (char *)self + 0x174);
    *out = *(unsigned int *)((char *)self + 0x174) - 1;
}
