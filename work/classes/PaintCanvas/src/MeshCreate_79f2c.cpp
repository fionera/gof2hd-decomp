#include "class.h"

extern "C" void paintcanvas_ext_mc_matcreate(void *self, unsigned short id, unsigned int *out);
extern "C" int paintcanvas_ext_mc_meshcreate(void *eng, unsigned short a, unsigned short b,
                                             signed char c, void **out);

namespace AbyssEngine {
void ArrayAddMesh(Mesh *item, void *array);
}

void MeshCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, unsigned short param_2,
                signed char param_3, unsigned short param_4, unsigned int *param_6)
{
    int result = -1;
    unsigned int mat = 0xffffffff;
    void *mesh = 0;
    paintcanvas_ext_mc_matcreate(self, param_4, &mat);
    int ok = paintcanvas_ext_mc_meshcreate(*(void **)((char *)self + 0x34), param_1, param_2,
                                           param_3, &mesh);
    if (ok == 1) {
        if (0xfffffffe < *(unsigned int *)((char *)self + 0x174)) {
            // material pointer back-reference: write last element ptr into mesh+0x30
            void *m = *(void **)(*(char **)((char *)self + 0x178) - 4);
            if (mesh) {
                *(void **)((char *)mesh + 0x30) = m;
            }
        }
        AbyssEngine::ArrayAddMesh((AbyssEngine::Mesh *)mesh, (char *)self + 0x24);
        result = *(int *)((char *)self + 0x24) - 1;
    }
    *param_6 = (unsigned int)result;
}
