#include "quaternion.h"

namespace AbyssEngine {

float Quaternion::Length() const {
    float length = x * x;
    length += y * y;
    length += z * z;
    length += w * w;
    return sqrtf(length);
}

} // namespace AbyssEngine
