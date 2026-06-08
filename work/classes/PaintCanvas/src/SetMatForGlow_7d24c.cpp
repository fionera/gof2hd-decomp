#include "class.h"

namespace AbyssEngine {
void ArrayAddMesh(Mesh *item, void *array);
void ArrayAddUint(unsigned int item, void *array);
}

// Adds a matrix (15 floats) plus the target Array pointer onto a glow-matrix list.
extern "C" void paintcanvas_ext_smfg_pushmat(const float *m, void *array);

void SetMatForGlow(AbyssEngine::PaintCanvas *self, char *param_1)
{
    char *t = (char *)self;
    int off = 0x38;
    for (unsigned int i = 0; i < *(unsigned int *)(param_1 + 0x44); i++) {
        // meshes array at 0x48 -> list at this+0x18c
        AbyssEngine::ArrayAddMesh((*(AbyssEngine::Mesh ***)(param_1 + 0x48))[i], t + 0x18c);

        const float *m1 = (const float *)(*(char **)(param_1 + 0x30) + off - 0x38);
        paintcanvas_ext_smfg_pushmat(m1, t + 0x198);

        const float *m2 = (const float *)(*(char **)(param_1 + 0x3c) + off - 0x38);
        paintcanvas_ext_smfg_pushmat(m2, t + 0x1a4);

        AbyssEngine::ArrayAddUint((*(unsigned int **)(param_1 + 0x54))[i], t + 0x1b0);

        const float *m3 = (const float *)(*(char **)(param_1 + 0x60) + off - 0x38);
        paintcanvas_ext_smfg_pushmat(m3, t + 0x1bc);

        off += 0x3c;
    }
}
