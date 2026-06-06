#include "quaternion.h"

extern "C" float sinf(float) __attribute__((longcall));
extern "C" float cosf(float) __attribute__((longcall));

namespace AbyssEngine {

void Quaternion::Set(float x_angle, float y_angle, float z_angle) {
    const float half = 0.5f;
    float (*sin_fn)(float) = sinf;
    float (*cos_fn)(float) = cosf;
    float sz = sin_fn(z_angle * half);
    float sy = sin_fn(y_angle * half);
    float sx = sin_fn(x_angle * half);
    float cz = cos_fn(z_angle * half);
    float cy = cos_fn(y_angle * half);
    float cx = cos_fn(x_angle * half);

    x = sx * cz * cy - sz * sy * cx;
    y = -(sy * cz * cx) + -(sx * sz * cy);
    z = sz * cy * cx - sx * sy * cz;
    w = cz * cy * cx + sz * sy * sx;
}

} // namespace AbyssEngine
