#include "class.h"

// AbyssEngine::AEMath::BSphere::operator=(BSphere const&)
// Plain 20-byte copy (center + both radii).

namespace AbyssEngine {
namespace AEMath {

BSphere &BSphere::operator=(const BSphere &other)
{
    this->cx = other.cx;
    this->cy = other.cy;
    this->cz = other.cz;
    this->radius = other.radius;
    this->maxRadius = other.maxRadius;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine
