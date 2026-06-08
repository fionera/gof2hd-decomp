#include "class.h"

// AbyssEngine::MeshConvertToVBO(AbyssEngine::Mesh*)
// Convert a single mesh to a VBO when VBOs are globally enabled and the mesh is eligible
// (flag@0x5c clear and pointer@0x84 set). Returns 1 on success, -4 (0xfffffffc) otherwise.
namespace AbyssEngine {

extern "C" char *g_Mesh_vboEnabledFlag; // *(DAT_0007bbe8 + 0x7bbbe)

void MeshConvertToVBOIntern(Mesh *mesh);
int TransformConvertToVBO(Transform *t);

int MeshConvertToVBO(Mesh *mesh)
{
    int result = -4;
    if (mesh != 0 && *g_Mesh_vboEnabledFlag != 0) {
        if (u8(mesh, 0x5c) != 0 || u8(mesh, 0x84) == 0)
            return -4;
        MeshConvertToVBOIntern(mesh);
        TransformConvertToVBO(*(Transform **)((char *)mesh + 0x34));
        result = 1;
    }
    return result;
}

} // namespace AbyssEngine
