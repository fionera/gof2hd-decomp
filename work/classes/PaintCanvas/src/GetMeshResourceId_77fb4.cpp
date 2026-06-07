#include "class.h"

extern "C" int paintcanvas_ext_strcmp(void *, void *);

unsigned int GetMeshResourceId(AbyssEngine::PaintCanvas *self, void *name, unsigned short p2)
{
    for (unsigned int i = 0; i < *(unsigned int *)((char *)self + 0x134); ++i) {
        char *res = (*(char ***)((char *)self + 0x138))[i];
        if (res && *(int *)(res + 0x4) == 4) {
            if (paintcanvas_ext_strcmp(name, **(void ***)(res + 0xc)) == 0) {
                char *res2 = (*(char ***)((char *)self + 0x138))[i];
                if (*(unsigned short *)(*(char **)(res2 + 0xc) + 0x4) == p2) {
                    return *(unsigned short *)res2;
                }
            }
        }
    }
    return 0xffff;
}
