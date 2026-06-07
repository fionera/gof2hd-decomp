#include "class.h"

extern "C" void paintcanvas_ext_mesh_changemat(AbyssEngine::PaintCanvas *, void *, void *);
extern "C" void paintcanvas_ext_transform_changemat(AbyssEngine::PaintCanvas *, void *, void *);

void MeshChangeMaterialIntern(AbyssEngine::PaintCanvas *self, char *transform, void *material)
{
    if (transform && material) {
        for (unsigned int i = 0; i < *(unsigned int *)(transform + 0x3c); ++i) {
            void *m = (*(void ***)(transform + 0x40))[i];
            paintcanvas_ext_mesh_changemat(self, m, material);
        }
        for (unsigned int i = 0; i < *(unsigned int *)(transform + 0x4c); ++i) {
            void *c = (*(void ***)(transform + 0x50))[i];
            paintcanvas_ext_transform_changemat(self, c, material);
        }
    }
}
