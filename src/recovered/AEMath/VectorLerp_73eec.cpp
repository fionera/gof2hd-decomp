#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector VectorLerp(const Vector &from, const Vector &to, float t)
{
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    float dz = to.z - from.z;
    Vector result;

    result.x = from.x + dx * t;
    result.y = from.y + dy * t;
    result.z = from.z + dz * t;

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
