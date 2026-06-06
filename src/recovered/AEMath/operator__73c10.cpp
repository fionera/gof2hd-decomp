#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector operator/(float lhs, const Vector &rhs)
{
    float x = rhs.x;
    float y = rhs.y;
    float z = rhs.z;
    float scale = lhs;
    Vector result;
    result.x = x / scale;
    result.y = y / scale;
    result.z = z / scale;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
