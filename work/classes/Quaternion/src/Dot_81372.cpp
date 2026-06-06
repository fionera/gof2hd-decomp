#include "quaternion.h"

namespace AbyssEngine {

float Quaternion::Dot(const Quaternion &a, const Quaternion &b) {
    float result = a.y * b.y;
    result += a.x * b.x;
    result += a.z * b.z;
    result += a.w * b.w;
    return result;
}

} // namespace AbyssEngine
