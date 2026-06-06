#include "quaternion.h"

namespace AbyssEngine {

extern "C" float Dot(const Quaternion *a, const Quaternion *b) {
    const volatile float *b_y = &b->y;
    float by = *b_y;
    const volatile float *a_y = &a->y;
    float ay = *a_y;
    const volatile float *b_x = &b->x;
    float bx = *b_x;
    float result = ay * by;
    const volatile float *a_x = &a->x;
    float ax = *a_x;
    const volatile float *b_z = &b->z;
    float bz = *b_z;
    const volatile float *a_z = &a->z;
    float az = *a_z;
    const volatile float *b_w = &b->w;
    float bw = *b_w;
    const volatile float *a_w = &a->w;
    float aw = *a_w;
    result += ax * bx;
    result += az * bz;
    result += aw * bw;
    return result;
}

} // namespace AbyssEngine
