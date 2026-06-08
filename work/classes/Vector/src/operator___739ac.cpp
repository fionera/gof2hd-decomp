#include "class.h"

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
