#include "class.h"

extern "C" int paintcanvas_ext_mesh_tricount(AbyssEngine::PaintCanvas *, void *);
extern "C" int paintcanvas_ext_transform_tricount(AbyssEngine::PaintCanvas *, void *);

int TransformGetTriCount(AbyssEngine::PaintCanvas *self, char *transform)
{
    if (!transform) {
        return 0;
    }
    int total = 0;
    unsigned int n1 = *(unsigned int *)(transform + 0x3c);
    for (unsigned int i = 0; i != n1; ++i) {
        void *m = (*(void ***)(transform + 0x40))[i];
        total += paintcanvas_ext_mesh_tricount(self, m);
    }
    unsigned int n2 = *(unsigned int *)(transform + 0x4c);
    for (unsigned int i = 0; i != n2; ++i) {
        void *c = (*(void ***)(transform + 0x50))[i];
        total += paintcanvas_ext_transform_tricount(self, c);
    }
    return total;
}
