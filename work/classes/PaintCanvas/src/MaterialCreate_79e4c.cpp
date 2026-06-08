#include "class.h"

extern "C" void *paintcanvas_ext_matc_findres(void *self, unsigned short id);
extern "C" void *paintcanvas_ext_matc_new_material();
extern "C" void paintcanvas_ext_matc_texcreate(void *self, unsigned short id, bool b);
extern "C" void paintcanvas_ext_matc_vec_assign(void *dst, void *src);

namespace AbyssEngine {
void ArrayAddMaterial(Material *item, void *array);
}

void MaterialCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, unsigned int *param_2)
{
    char *res = (char *)paintcanvas_ext_matc_findres(self, param_1);
    if (res == 0) {
        return;
    }
    unsigned int idx = *(unsigned int *)(res + 8);
    if (idx == 0xffffffff) {
        char *info = *(char **)(res + 0xc);
        char *mat = (char *)paintcanvas_ext_matc_new_material();
        for (unsigned int i = 0; i < 8; i++) {
            unsigned short tid = *(unsigned short *)(info + i * 2);
            if (tid != 0xffff) {
                char *tres = (char *)paintcanvas_ext_matc_findres(self, tid);
                if (tres == 0) {
                    break;
                }
                int t = *(int *)(tres + 8);
                if (t == -1) {
                    paintcanvas_ext_matc_texcreate(self, *(unsigned short *)(info + i * 2), true);
                    t = *(int *)(tres + 8);
                }
                *(int *)(mat + i * 4) = t;
            }
        }
        *(unsigned int *)(mat + 0x20) = *(unsigned int *)(info + 0x10);
        *(unsigned int *)(mat + 0x24) = *(unsigned int *)(info + 0x14);
        *(unsigned int *)(mat + 0x28) = *(unsigned int *)(info + 0x18);
        paintcanvas_ext_matc_vec_assign(mat + 0x68, info + 0x1c);
        AbyssEngine::ArrayAddMaterial((AbyssEngine::Material *)mat, (char *)self + 0x174);
        idx = *(unsigned int *)((char *)self + 0x174) - 1;
        *(unsigned int *)(res + 8) = idx;
    }
    *param_2 = idx;
}
