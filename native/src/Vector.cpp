#include "gof2/Vector.h"




// ---- operator___73a08.cpp ----
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

// ---- operator___73960.cpp ----
namespace AbyssEngine {
namespace AEMath {

// AbyssEngine::AEMath::Vector::operator[](int) -> reference to component.
float &Vector::operator[](int index)
{
    return (&x)[index];
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___739ac.cpp ----
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

// ---- operator___73a64.cpp ----
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

// ---- operator__7396c.cpp ----
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
