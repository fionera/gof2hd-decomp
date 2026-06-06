#include "quaternion.h"

namespace AbyssEngine {

float Quaternion::Dot(const Quaternion &a, const Quaternion &b) {
    return a.y * b.y + a.x * b.x + a.z * b.z + a.w * b.w;
}

} // namespace AbyssEngine
