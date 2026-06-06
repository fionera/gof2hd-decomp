#include "quaternion.h"

namespace AbyssEngine {

void Quaternion::Set(const AEMath::Matrix &matrix) {
    const float one = 1.0f;
    const float epsilon = 0.0000001f;

    float m00 = matrix.m[0];
    float m11 = matrix.m[5];
    float m22 = matrix.m[10];
    float trace = ((m00 + m11) + m22) + one;

    if (trace > epsilon) {
        float scale = sqrtf(trace);
        scale = (one / scale) * 0.5f;

        w = 0.25f / scale;
        x = scale * (matrix.m[6] - matrix.m[9]);
        y = scale * (matrix.m[8] - matrix.m[2]);
        z = scale * (matrix.m[1] - matrix.m[4]);
    } else if (m00 > m11 && m00 > m22) {
        float scale = sqrtf(((m00 + one) - m11) - m22);
        scale = scale + scale;

        float w_value = (matrix.m[9] - matrix.m[6]) / scale;
        x = scale * 0.25f;
        w = w_value;
        y = (matrix.m[4] + matrix.m[1]) / scale;
        z = (matrix.m[8] + matrix.m[2]) / scale;
    } else if (m11 > m22) {
        float scale = sqrtf(((m11 + one) - m00) - m22);
        scale = scale + scale;

        w = (matrix.m[8] - matrix.m[2]) / scale;
        x = (matrix.m[4] + matrix.m[1]) / scale;
        y = scale * 0.25f;
        z = (matrix.m[9] + matrix.m[6]) / scale;
    } else {
        float scale = sqrtf(((m22 + one) - m00) - m11);
        scale = scale + scale;

        w = (matrix.m[4] - matrix.m[1]) / scale;
        x = (matrix.m[8] + matrix.m[2]) / scale;
        y = (matrix.m[9] + matrix.m[6]) / scale;
        z = scale * 0.25f;
    }
}

} // namespace AbyssEngine
