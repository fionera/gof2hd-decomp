#include "class.h"

// AbyssEngine::MeshRelease(AbyssEngine::Engine*, AbyssEngine::Mesh**)
namespace AbyssEngine {

// Sibling free functions (resolved by the linker via their AbyssEngine-mangled names).
void *TransformRelease(Engine *engine, Transform **slot);

void MeshRelease(Engine *engine, Mesh **slot)
{
    if (engine != 0 && *slot != 0) {
        void *r = TransformRelease(engine, (Transform **)((char *)*slot + 0x34));
        // Tail-call through a runtime function pointer that frees the mesh body.
        typedef void (*FreeFn)(void *, Mesh **);
        extern void *g_MeshRelease_freeFn; // *(DAT_001ab034 + 0x1ab038)
        ((FreeFn)g_MeshRelease_freeFn)(r, slot);
    }
}

} // namespace AbyssEngine
