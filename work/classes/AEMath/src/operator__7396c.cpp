#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector &Vector::operator=(const Vector &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine
