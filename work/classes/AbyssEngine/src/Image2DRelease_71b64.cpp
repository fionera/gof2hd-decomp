#include "class.h"

void operator delete(void *ptr) noexcept;

// AbyssEngine::Image2DRelease(AbyssEngine::Engine*, AbyssEngine::Image2D**)
namespace AbyssEngine {

void MeshRelease(Engine *engine, Mesh **slot);

void Image2DRelease(Engine *engine, Image2D **slot)
{
    if (*slot != 0) {
        MeshRelease(engine, (Mesh **)*slot);
        if (*slot != 0)
            operator delete((void *)*slot);
        *slot = 0;
    }
}

} // namespace AbyssEngine
