#include "class.h"

extern "C" void paintcanvas_ext_change_mat(void *, void *, void *);

void MeshChangeMaterial(AbyssEngine::PaintCanvas *self, unsigned int meshIndex, unsigned int matIndex)
{
    if (matIndex < *(unsigned int *)((char *)self + 0x174) &&
        meshIndex < *(unsigned int *)((char *)self + 0x24)) {
        void *mesh = (*(void ***)((char *)self + 0x28))[meshIndex];
        void *mat = (*(void ***)((char *)self + 0x178))[matIndex];
        return paintcanvas_ext_change_mat(self, mesh, mat);
    }
}
