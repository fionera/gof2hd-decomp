#include "class.h"

extern "C" void paintcanvas_ext_shader_anim(void *, void *);

void MeshChangeShaderAnimValue(AbyssEngine::PaintCanvas *self, char *mesh, float value, unsigned int mode)
{
    if (!mesh) {
        return;
    }
    float *p;
    if (mode == 4) {
        p = (float *)(mesh + 0x24);
    } else if (mode == 2) {
        p = (float *)(mesh + 0x20);
    } else {
        if (mode != 1) {
            goto tail;
        }
        p = (float *)(mesh + 0x1c);
    }
    *p = value;
tail:
    return paintcanvas_ext_shader_anim(self, *(void **)(mesh + 0x34));
}
