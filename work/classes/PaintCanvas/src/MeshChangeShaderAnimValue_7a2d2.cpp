#include "class.h"

extern "C" void paintcanvas_ext_mesh_shaderanim(AbyssEngine::PaintCanvas *, void *, float, unsigned int);
extern "C" void paintcanvas_ext_transform_shaderanim(AbyssEngine::PaintCanvas *, void *, float, unsigned int);

void MeshChangeShaderAnimValue(AbyssEngine::PaintCanvas *self, char *transform, float value, unsigned int mode)
{
    if (!transform) {
        return;
    }
    for (unsigned int i = 0; i < *(unsigned int *)(transform + 0x3c); ++i) {
        void *m = (*(void ***)(transform + 0x40))[i];
        paintcanvas_ext_mesh_shaderanim(self, m, value, mode);
    }
    for (unsigned int i = 0; i < *(unsigned int *)(transform + 0x4c); ++i) {
        void *c = (*(void ***)(transform + 0x50))[i];
        paintcanvas_ext_transform_shaderanim(self, c, value, mode);
    }
}
