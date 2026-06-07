#include "class.h"

extern "C" int paintcanvas_ext_transform_tricount(void *, void *);
extern "C" unsigned int __aeabi_uidiv(unsigned int, unsigned int);

int MeshGetTriCount(AbyssEngine::PaintCanvas *self, char *mesh)
{
    if (mesh) {
        void *t = *(void **)(mesh + 0x34);
        int tri = t ? paintcanvas_ext_transform_tricount(self, t) : 0;
        return tri + (int)__aeabi_uidiv(*(unsigned short *)(mesh + 0x28), 3);
    }
    return 0;
}
