#include "class.h"

extern "C" void *paintcanvas_ext_alloc(unsigned int);
extern "C" void *paintcanvas_ext_transform_ctor(void *);
extern "C" void paintcanvas_ext_add_child(void *, void *);

void TransformCreate(AbyssEngine::PaintCanvas *self, unsigned int *out)
{
    void *obj = paintcanvas_ext_alloc(0x180);
    paintcanvas_ext_transform_ctor(obj);
    paintcanvas_ext_add_child(obj, (char *)self + 0x158);
    *out = *(unsigned int *)((char *)self + 0x158) - 1;
}
