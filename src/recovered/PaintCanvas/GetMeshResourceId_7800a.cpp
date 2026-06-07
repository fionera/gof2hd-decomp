#include "class.h"

extern "C" int paintcanvas_ext_strcmp(void *, void *);

unsigned int GetMeshResourceId(AbyssEngine::PaintCanvas *self, void *name)
{
    for (unsigned int i = 0; i < *(unsigned int *)((char *)self + 0x134); ++i) {
        char *res = (*(char ***)((char *)self + 0x138))[i];
        if (res && *(int *)(res + 0x4) == 4) {
            char *n = *(char **)(res + 0xc);
            if (paintcanvas_ext_strcmp(name, *(void **)n) == 0) {
                return *(unsigned short *)((*(char ***)((char *)self + 0x138))[i]);
            }
        }
    }
    return 0xffff;
}
