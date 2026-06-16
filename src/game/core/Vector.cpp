#include "game/core/Vector.h"

namespace AbyssEngine {
namespace AEMath {

// AbyssEngine::AEMath::Vector::operator*=(const Vector&) -> component-wise multiply.
Vector &Vector::operator*=(const Vector &rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine

// AbyssEngine::AEMath::Vector::operator[](int) is defined inline in gof2/math.h.

namespace AbyssEngine {
namespace AEMath {

// AbyssEngine::AEMath::Vector::operator-=(const Vector&) -> component-wise subtract.
Vector &Vector::operator-=(const Vector &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine

namespace AbyssEngine {
namespace AEMath {

// AbyssEngine::AEMath::Vector::operator/=(const Vector&) -> component-wise divide.
Vector &Vector::operator/=(const Vector &rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine

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
