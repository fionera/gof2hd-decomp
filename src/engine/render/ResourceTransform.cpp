#include "engine/render/ResourceTransform.h"

namespace AbyssEngine {

// Releases the two owned heap arrays and nulls the slots.
ResourceTransform::~ResourceTransform()
{
    ::operator delete[](dataA);
    dataA = nullptr;

    ::operator delete[](dataB);
    dataB = nullptr;
}

} // namespace AbyssEngine
