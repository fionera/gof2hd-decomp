#include "class.h"

extern "C" void *paintcanvas_ext_tfc_findres(void *self, unsigned short id);
extern "C" void *paintcanvas_ext_tfc_new_transform();
extern "C" void paintcanvas_ext_tfc_mtx_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_tfc_meshcreate(void *self, unsigned short id, unsigned int *out, bool b);

namespace AbyssEngine {
void ArrayAddTransform(Transform *item, void *array);
}

void TransformCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, unsigned int *param_2)
{
    char *res = (char *)paintcanvas_ext_tfc_findres(self, param_1);
    if (res == 0) {
        return;
    }
    if (*(unsigned int *)(res + 8) != 0xffffffff) {
        *param_2 = *(unsigned int *)(res + 8);
        return;
    }
    char *info = *(char **)(res + 0xc);
    char *tf = (char *)paintcanvas_ext_tfc_new_transform();
    AbyssEngine::ArrayAddTransform((AbyssEngine::Transform *)tf, (char *)self + 0x158);
    unsigned int idx = *(unsigned int *)((char *)self + 0x158) - 1;
    *(unsigned int *)(res + 8) = idx;
    *param_2 = idx;
    paintcanvas_ext_tfc_mtx_assign(tf, info);

    unsigned int childMesh = 0xffffffff;
    for (unsigned int i = 0; i < *(unsigned short *)(info + 0x3c); i++) {
        unsigned short mid = *(unsigned short *)(*(char **)(info + 0x40) + i * 2);
        paintcanvas_ext_tfc_meshcreate(self, mid, &childMesh, false);
    }
    unsigned int childTf = 0xffffffff;
    for (unsigned int i = 0; i < *(unsigned short *)(info + 0x44); i++) {
        unsigned short tid = *(unsigned short *)(*(char **)(info + 0x48) + i * 2);
        TransformCreate(self, tid, &childTf);
    }
}
