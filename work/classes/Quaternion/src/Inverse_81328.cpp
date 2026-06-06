#include "quaternion.h"

namespace AbyssEngine {

extern "C" void Inverse(Quaternion *result, const Quaternion *self) {
    const volatile float *y_ptr = &self->y;
    float y_value = *y_ptr;
    float x_value = self->x;
    float z_value = self->z;
    float w_value = self->w;
    float length_sq = y_value * y_value;
    length_sq += x_value * x_value;
    length_sq += z_value * z_value;
    length_sq += w_value * w_value;
    float inv_length_sq = 1.0f / length_sq;
    result->x = -(inv_length_sq * x_value);
    result->y = -(inv_length_sq * y_value);
    result->z = -(inv_length_sq * z_value);
    result->w = w_value * inv_length_sq;
}

} // namespace AbyssEngine
