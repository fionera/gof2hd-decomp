#include "class.h"

extern "C" int paintcanvas_ext_meshcreate(void *, void *);
extern "C" void paintcanvas_ext_array_add_mesh(void *, void *);

void MeshCreate(AbyssEngine::PaintCanvas *self, unsigned short a, unsigned short b,
                signed char c, unsigned int *out)
{
    char mesh[4];
    *(void **)mesh = 0;
    int result = paintcanvas_ext_meshcreate(*(void **)((char *)self + 0x34), mesh);
    if (result == 1) {
        paintcanvas_ext_array_add_mesh(*(void **)mesh, (char *)self + 0x24);
        result = (int)*(unsigned int *)((char *)self + 0x24) - 1;
    } else {
        result = -1;
    }
    *out = result;
}
