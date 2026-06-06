#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector VectorNormalize(const Vector &value) {
    float lengthSquared = value.y * value.y;
    lengthSquared += value.x * value.x;
    lengthSquared += value.z * value.z;
    const float length = sqrtf(lengthSquared);
    Vector result;
    if (length == 0.0f) {
        result.x = 0.0f;
        result.y = 1.0f;
        result.z = 0.0f;
    } else {
        result.x = value.x / length;
        result.y = value.y / length;
        result.z = value.z / length;
    }
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
