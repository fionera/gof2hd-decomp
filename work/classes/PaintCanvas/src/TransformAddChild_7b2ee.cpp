#include "class.h"

extern "C" void paintcanvas_ext_array_add(void *, void *);
extern "C" void paintcanvas_ext_child_link(void *, void *, void *);
extern "C" void paintcanvas_ext_transform_dirty(void *);

void TransformAddChild(AbyssEngine::PaintCanvas *self, unsigned int parent, unsigned int child)
{
    unsigned int count = *(unsigned int *)((char *)self + 0x158);
    if (parent != child && child < count && parent < count) {
        char **arr = *(char ***)((char *)self + 0x15c);
        char *p = arr[parent];
        char *c = arr[child];
        paintcanvas_ext_array_add(c, p + 0x4c);
        char **arr2 = *(char ***)((char *)self + 0x15c);
        char *p2 = arr2[parent];
        char *c2 = arr2[child];
        paintcanvas_ext_child_link(p2 + 0xd4, c2, p2);
        char **arr3 = *(char ***)((char *)self + 0x15c);
        return paintcanvas_ext_transform_dirty(arr3[parent]);
    }
}
