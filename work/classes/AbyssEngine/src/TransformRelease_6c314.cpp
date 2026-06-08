#include "class.h"

// AbyssEngine::TransformRelease(AbyssEngine::Engine*, AbyssEngine::Transform**)
// Recursively release child transforms (count@0x4c, array@0x50) and owned meshes
// (count@0x3c, array@0x40).
namespace AbyssEngine {

void MeshRelease(Engine *engine, Mesh **slot);

void TransformRelease(Engine *engine, Transform **slot)
{
    Transform *t = (Transform *)*slot;
    if (t == 0)
        return;

    int off = 0;
    for (unsigned int i = 0; i < u32(t, 0x4c); ++i) {
        TransformRelease(engine, (Transform **)((char *)pp(t, 0x50) + off));
        off += 4;
        t = (Transform *)*slot;
    }

    off = 0;
    for (unsigned int i = 0; i < u32(t, 0x3c); ++i) {
        MeshRelease(engine, (Mesh **)((char *)pp(t, 0x40) + off));
        off += 4;
        t = (Transform *)*slot;
    }
}

} // namespace AbyssEngine
