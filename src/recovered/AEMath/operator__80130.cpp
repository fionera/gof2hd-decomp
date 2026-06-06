#include "class.h"
#include <arm_neon.h>

namespace AbyssEngine {
namespace AEMath {

Matrix &Matrix::operator=(const Matrix &other) {
    float32x4_t values = vld1q_f32(other.m);
    vst1q_f32(m, values);

    values = vld1q_f32(other.m + 4);
    vst1q_f32(m + 4, values);

    values = vld1q_f32(other.m + 8);
    vst1q_f32(m + 8, values);

    *reinterpret_cast<Vector *>(m + 12) =
        *reinterpret_cast<const Vector *>(other.m + 12);
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine
