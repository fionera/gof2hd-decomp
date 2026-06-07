#include "class.h"

extern "C" int paintcanvas_ext_transform_tricount(void *, void *);
extern "C" unsigned int __aeabi_uidiv(unsigned int, unsigned int);

int MeshGetTriCount(AbyssEngine::PaintCanvas *self, char *mesh)
{
    if (mesh) {
        int tri;
        if (*(void **)(mesh + 0x34) == 0) {
            tri = 0;
        } else {
            tri = paintcanvas_ext_transform_tricount(self, *(void **)(mesh + 0x34));
        }
        int q = (int)__aeabi_uidiv(*(unsigned short *)(mesh + 0x28), 3);
        return q + tri;
    }
    return 0;
}
