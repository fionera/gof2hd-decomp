#include "class.h"

void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;

// AbyssEngine::ImageRelease(AbyssEngine::Image**)
namespace AbyssEngine {

void ImageRelease(Image **slot)
{
    if (*slot != 0) {
        operator delete[](pp(*slot, 0xc));
        pp(*slot, 0xc) = 0;
        if (*slot != 0)
            operator delete((void *)*slot);
        *slot = 0;
    }
}

} // namespace AbyssEngine
