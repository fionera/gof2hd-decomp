#include "class.h"

extern "C" void paintcanvas_ext_mat_intern(void *, void *);

void MeshChangeMaterialIntern(AbyssEngine::PaintCanvas *self, void *mesh, void *mat)
{
    if (mesh && mat) {
        *(void **)((char *)mesh + 0x30) = mat;
        return paintcanvas_ext_mat_intern(self, *(void **)((char *)mesh + 0x34));
    }
}
