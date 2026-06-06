#include "quaternion.h"

namespace AbyssEngine {

extern "C" void AbyssEngine_Quaternion_Inverse(Quaternion *result, const Quaternion *self) {
    float x = self->x;
    float y = self->y;
    float z = self->z;
    float w = self->w;
    float inv = 1.0f / (y * y + x * x + z * z + w * w);
    float ninv = -inv;
    result->x = ninv * x;
    result->y = ninv * y;
    result->z = ninv * z;
    result->w = w * inv;
}

} // namespace AbyssEngine
