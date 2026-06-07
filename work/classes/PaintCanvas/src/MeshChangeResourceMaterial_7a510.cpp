#include "class.h"

extern "C" char *paintcanvas_ext_find_res(void *, unsigned int);
extern "C" void paintcanvas_ext_change_mat(void *, void *, void *);

void MeshChangeResourceMaterial(AbyssEngine::PaintCanvas *self, unsigned int meshIndex, unsigned int resId)
{
    char *r = paintcanvas_ext_find_res(self, resId);
    if (r) {
        int idx = *(int *)(r + 0x8);
        if (idx + 1 != 0) {
            void *mesh = (*(void ***)((char *)self + 0x28))[meshIndex];
            void *mat = (*(void ***)((char *)self + 0x178))[idx];
            return paintcanvas_ext_change_mat(self, mesh, mat);
        }
    }
}
