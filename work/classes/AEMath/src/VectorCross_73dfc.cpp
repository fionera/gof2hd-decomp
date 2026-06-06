#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector VectorCross(const Vector &lhs, const Vector &rhs) {
    Vector result;
    float y = lhs.z * rhs.x;
    float z = rhs.y * lhs.x;
    float x = lhs.y * rhs.z;
    y -= rhs.z * lhs.x;
    z -= lhs.y * rhs.x;
    x -= lhs.z * rhs.y;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
