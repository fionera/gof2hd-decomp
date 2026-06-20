#include "engine/render/ResourceTransform.h"

namespace AbyssEngine {

// Releases the two owned heap arrays and nulls the slots.
ResourceTransform::~ResourceTransform()
{
    if (dataA != nullptr) {
        ::operator delete[](dataA);
    }
    dataA = nullptr;

    if (dataB != nullptr) {
        ::operator delete[](dataB);
    }
    dataB = nullptr;
}

} // namespace AbyssEngine
