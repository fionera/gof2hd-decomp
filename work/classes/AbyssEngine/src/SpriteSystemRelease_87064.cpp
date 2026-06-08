#include "class.h"

void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;

// AbyssEngine::SpriteSystemRelease(AbyssEngine::Engine*, AbyssEngine::SpriteSystem**)
namespace AbyssEngine {

void MeshRelease(Engine *engine, Mesh **slot);

void SpriteSystemRelease(Engine *engine, SpriteSystem **slot)
{
    if (*slot == 0)
        return;

    void *p = pp(*slot, 0x4);
    if (p != 0)
        operator delete[](p);
    pp(*slot, 0x4) = 0;

    p = pp(*slot, 0x8);
    if (p != 0)
        operator delete[](p);
    pp(*slot, 0x8) = 0;

    MeshRelease(engine, (Mesh **)((char *)*slot + 0x10));

    if (*slot != 0)
        operator delete((void *)*slot);
    *slot = 0;
}

} // namespace AbyssEngine
