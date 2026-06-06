#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector operator/(const Vector &lhs, float rhs)
{
    float x = lhs.x;
    float y = lhs.y;
    float z = lhs.z;
    Vector result;
    result.x = x / rhs;
    result.y = y / rhs;
    result.z = z / rhs;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
