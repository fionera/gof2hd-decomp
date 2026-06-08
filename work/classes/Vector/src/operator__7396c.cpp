#include "class.h"

namespace AbyssEngine {
namespace AEMath {

// AbyssEngine::AEMath::Vector::operator=(const Vector&) -> copy all three components.
Vector &Vector::operator=(const Vector &rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine
