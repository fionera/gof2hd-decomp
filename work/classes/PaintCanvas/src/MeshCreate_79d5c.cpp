#include "class.h"

__attribute__((visibility("hidden"))) extern char *const g_meshcreate_vboflag_79d5c;

extern "C" void *paintcanvas_ext_mc2_findres(void *self, unsigned short id);
extern "C" void paintcanvas_ext_mc2_matcreate(void *self, unsigned short id, unsigned int *out);
extern "C" int paintcanvas_ext_mc2_meshfromfile(void *eng, char *path, void **out, void *mat);
extern "C" void *paintcanvas_ext_mc2_new_mesh_copy(void *src);
extern "C" void paintcanvas_ext_mc2_converttovbo(void *mesh);

namespace AbyssEngine {
void ArrayAddMesh(Mesh *item, void *array);
}

void MeshCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, unsigned int *param_2,
                bool param_3)
{
    char *res = (char *)paintcanvas_ext_mc2_findres(self, param_1);
    if (res == 0) {
        return;
    }
    unsigned int idx = *(unsigned int *)(res + 8);
    if (idx == 0xffffffff) {
        char *info = *(char **)(res + 0xc);
        unsigned int mat = 0xffffffff;
        paintcanvas_ext_mc2_matcreate(self, *(unsigned short *)(info + 4), &mat);
        void *matptr = 0;
        if (0xfffffffe < *(unsigned int *)((char *)self + 0x174)) {
            matptr = *(void **)(*(char **)((char *)self + 0x178) - 4);
        }
        void *mesh = 0;
        int ok = paintcanvas_ext_mc2_meshfromfile(*(void **)((char *)self + 0x34),
                                                  *(char **)info, &mesh, matptr);
        if (ok != 1) {
            return;
        }
        if (*g_meshcreate_vboflag_79d5c != 0) {
            if (mesh) {
                *(unsigned char *)((char *)mesh + 0x84) = 1;
            }
            paintcanvas_ext_mc2_converttovbo(mesh);
        }
        AbyssEngine::ArrayAddMesh((AbyssEngine::Mesh *)mesh, (char *)self + 0x24);
        idx = *(unsigned int *)((char *)self + 0x24) - 1;
        *(unsigned int *)(res + 8) = idx;
    } else {
        char *meshes = *(char **)((char *)self + 0x28);
        char *existing = ((char **)meshes)[idx];
        if (*(int *)(existing + 0x34) != 0 || param_3) {
            void *clone = paintcanvas_ext_mc2_new_mesh_copy(
                ((void **)meshes)[*(unsigned int *)(res + 8)]);
            AbyssEngine::ArrayAddMesh((AbyssEngine::Mesh *)clone, (char *)self + 0x24);
            idx = *(unsigned int *)((char *)self + 0x24) - 1;
        }
    }
    *param_2 = idx;
}
