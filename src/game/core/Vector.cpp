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

namespace AbyssEngine {
namespace AEMath {

// Conversion operators: expose the 3-float storage as a raw float array.
Vector::operator float*()             { return &x; }
Vector::operator const float*() const { return &x; }

// Indexed access into the {x,y,z} storage.
float &Vector::operator[](int i)       { return (&x)[i]; }
float  Vector::operator[](int i) const { return (&x)[i]; }

} // namespace AEMath
} // namespace AbyssEngine

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
