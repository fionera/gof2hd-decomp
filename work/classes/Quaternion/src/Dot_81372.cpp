#include "quaternion.h"

namespace AbyssEngine {

float Quaternion::Dot(const Quaternion &a, const Quaternion &b) {
    float by = b.y;
    float ay = a.y;
    float bx = b.x;
    float result = ay * by;
    float ax = a.x;
    float bz = b.z;
    float az = a.z;
    float bw = b.w;
    float aw = a.w;
    result += ax * bx;
    result += az * bz;
    result += aw * bw;
    return result;
}

} // namespace AbyssEngine
