#include "class.h"

extern "C" int paintcanvas_ext_transform_tricount(void *, void *);

int TransformGetTriCount(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < *(unsigned int *)((char *)self + 0x158)) {
        void *t = (*(void ***)((char *)self + 0x15c))[index];
        return paintcanvas_ext_transform_tricount(self, t);
    }
    return 0;
}
