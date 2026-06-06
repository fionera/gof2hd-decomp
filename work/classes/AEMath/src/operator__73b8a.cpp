#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector operator*(float lhs, const Vector &rhs)
{
    float x = rhs.x;
    float y = rhs.y;
    float z = rhs.z;
    Vector result;
    result.x = x * lhs;
    result.y = y * lhs;
    result.z = z * lhs;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
