#include "class.h"

extern "C" void paintcanvas_ext_array_remove(void *, void *);
extern "C" void paintcanvas_ext_transform_dirty(void *);

void TransformRemoveChild(AbyssEngine::PaintCanvas *self, unsigned int parent, unsigned int child)
{
    unsigned int count = *(unsigned int *)((char *)self + 0x158);
    if (parent != child && child < count && parent < count) {
        char **arr = *(char ***)((char *)self + 0x15c);
        char *p = arr[parent];
        char *c = arr[child];
        paintcanvas_ext_array_remove(c, p + 0x4c);
        char **arr2 = *(char ***)((char *)self + 0x15c);
        return paintcanvas_ext_transform_dirty(arr2[parent]);
    }
}
