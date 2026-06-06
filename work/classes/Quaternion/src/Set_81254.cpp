#include "quaternion.h"

extern "C" float (*gof2_sinf)(float);
extern "C" float (*gof2_cosf)(float);

namespace AbyssEngine {

void Quaternion::Set(float x_angle, float y_angle, float z_angle) {
    const float half = 0.5f;
    float (*sin_fn)(float) = gof2_sinf;
    float (*cos_fn)(float) = gof2_cosf;
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
