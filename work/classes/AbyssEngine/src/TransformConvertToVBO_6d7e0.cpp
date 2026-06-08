#include "class.h"

// AbyssEngine::TransformConvertToVBO(AbyssEngine::Transform*)
// Convert owned meshes to VBOs (count@0x3c, array@0x40), then recurse into child transforms
// (count@0x4c, array@0x50).
namespace AbyssEngine {

int MeshConvertToVBO(Mesh *mesh);

int TransformConvertToVBO(Transform *t)
{
    if (t != 0) {
        for (unsigned int i = 0; i < u32(t, 0x3c); ++i)
            MeshConvertToVBO(*(Mesh **)((char *)pp(t, 0x40) + i * 4));
        for (unsigned int i = 0; i < u32(t, 0x4c); ++i)
            TransformConvertToVBO(*(Transform **)((char *)pp(t, 0x50) + i * 4));
    }
    return 1;
}

} // namespace AbyssEngine
