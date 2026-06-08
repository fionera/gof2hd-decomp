#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector VectorNormalize(const Vector &value) {
    const float length = sqrtf(value.x * value.x + value.y * value.y + value.z * value.z);
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
