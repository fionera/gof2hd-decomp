#include "gof2/engine/math/Quaternion.h"
#include "gof2/externs.h"
#include <arm_neon.h>

// ---- Quaternion_8102a.cpp ----
namespace AbyssEngine {

Quaternion::Quaternion(Quaternion *other) {
    (void)other;
}

} // namespace AbyssEngine

// ---- Dot_81372.cpp ----
namespace AbyssEngine {

float Quaternion::Dot(const Quaternion &a, const Quaternion &b) {
    return a.y * b.y + a.x * b.x + a.z * b.z + a.w * b.w;
}

} // namespace AbyssEngine

// ---- Length_814e4.cpp ----
namespace AbyssEngine {

float Quaternion::Length() const {
    return sqrtf(x * x + y * y + z * z + w * w);
}

} // namespace AbyssEngine

// ---- Quaternion_81018.cpp ----
namespace AbyssEngine {

Quaternion::Quaternion(AEMath::Vector angles)
{
    Set(angles);
}

} // namespace AbyssEngine

// ---- Set_81026.cpp ----
namespace AbyssEngine {

void Quaternion::Set(AEMath::Vector angles) {
    Set(angles.x, angles.y, angles.z);
}

namespace {

__attribute__((used)) void Set_81026_trailer() {}

}

} // namespace AbyssEngine

// ---- operator[]_814de.cpp ----
// const operator[] is defined inline in math.h; no out-of-line definition needed.

// ---- Quaternion_8102c.cpp ----
namespace AbyssEngine {

Quaternion::Quaternion(const AEMath::Matrix &matrix) {
    Set(matrix);
}

} // namespace AbyssEngine

// ---- Normalized_81410.cpp ----
namespace AbyssEngine {

Quaternion Quaternion::Normalized() {
    float length = Length();
    x /= length;
    y /= length;
    z /= length;
    w /= length;
    return *this;
}

} // namespace AbyssEngine

// ---- operator.cast.to.float*_814d6.cpp ----
// operator const float* is defined inline in math.h.

// ---- Quaternion_81008.cpp ----
// default ctor is defined inline in math.h.

// ---- operator.cast.to.float*_814d4.cpp ----
// operator float* is defined inline in math.h.

// ---- Set_8103c.cpp ----
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

// ---- Inverse_81328.cpp ----
namespace AbyssEngine {

Quaternion Quaternion::Inverse() const {
    float inv = 1.0f / (x * x + y * y + z * z + w * w);
    Quaternion result;
    result.x = -(inv * x);
    result.y = -(inv * y);
    result.z = -(inv * z);
    result.w = w * inv;
    return result;
}

} // namespace AbyssEngine

// ---- Set_81254.cpp ----
namespace AbyssEngine {

void Quaternion::Set(float x_angle, float y_angle, float z_angle) {
    float sinZ = sinf(z_angle * 0.5f);
    float sinY = sinf(y_angle * 0.5f);
    float sinX = sinf(x_angle * 0.5f);
    float cosZ = cosf(z_angle * 0.5f);
    float cosY = cosf(y_angle * 0.5f);
    float cosX = cosf(x_angle * 0.5f);
    x = sinX * cosZ * cosY - sinZ * sinY * cosX;
    y = -(sinY * cosZ * cosX) + -(sinX * sinZ * cosY);
    z = sinZ * cosY * cosX - sinX * sinY * cosZ;
    w = cosZ * cosY * cosX + sinZ * sinY * sinX;
}

} // namespace AbyssEngine

// ---- Lerp_813a8.cpp ----
namespace AbyssEngine {

// Straight (non-shortest-path) lerp: result = a + t * (b - a), then normalize.
void Quaternion::Lerp(const Quaternion &a, const Quaternion &b, float t) {
    float32x4_t av = vld1q_f32(&a.x);
    float32x4_t bv = vld1q_f32(&b.x);
    float32x4_t delta = vsubq_f32(bv, av);
    float32x4_t result = vmlaq_f32(av, vdupq_n_f32(t), delta);

    vst1q_f32(&x, result);
    *this = Normalized();
}

} // namespace AbyssEngine

// ---- operator[]_814d8.cpp ----
// non-const operator[] is defined inline in math.h.

// ---- ~Quaternion_81250.cpp ----
// destructor is defined inline in math.h.

// ---- Convert_81520.cpp ----
namespace AbyssEngine {

void Quaternion::Convert(AEMath::Matrix &matrix) {
    float fVar10 = w;
    float fVar7 = x;
    float fVar8 = y;
    float fVar9 = z;
    double fVar14 = fVar7 * fVar7;
    double fVar11 = fVar8 * fVar8;
    double fVar12 = fVar9 * fVar9;
    double fVar13 = fVar10 * fVar10;
    double fVar1 = 1.0 / (fVar14 + fVar11 + fVar12 + fVar13);
    double fVar6 = (double)(fVar7 * fVar8) + (double)(fVar10 * fVar9);
    double fVar5 = (double)(fVar7 * fVar8) - (double)(fVar10 * fVar9);
    double fVar4 = (double)(fVar7 * fVar9) - (double)(fVar10 * fVar8);
    double fVar3 = (double)(fVar10 * fVar8) + (double)(fVar7 * fVar9);
    double fVar2 = (double)(fVar10 * fVar7) + (double)(fVar8 * fVar9);
    double fVar7b = (double)(fVar8 * fVar9) - (double)(fVar10 * fVar7);
    matrix.m[0] = (((fVar14 - fVar11) - fVar12) + fVar13) * fVar1;
    matrix.m[1] = (fVar6 + fVar6) * fVar1;
    matrix.m[2] = (fVar4 + fVar4) * fVar1;
    matrix.m[4] = (fVar5 + fVar5) * fVar1;
    matrix.m[5] = (((fVar11 - fVar14) - fVar12) + fVar13) * fVar1;
    matrix.m[6] = (fVar2 + fVar2) * fVar1;
    matrix.m[8] = (fVar3 + fVar3) * fVar1;
    matrix.m[9] = (fVar7b + fVar7b) * fVar1;
    matrix.m[10] = ((-fVar14 - fVar11) + fVar12 + fVar13) * fVar1;
}

} // namespace AbyssEngine

// ---- Quaternion_8100a.cpp ----
// 4-float ctor is defined inline in math.h.

// ---- Lerp_8145c.cpp ----
namespace AbyssEngine {

// Shortest-path lerp over raw float[4] quaternions: if the two point in opposite
// hemispheres (dot < 0) negate b first, then result = a + t * (delta), normalize.
void Quaternion::Lerp(const float *a, const float *b, float t) {
    float32x4_t av = vld1q_f32(a);
    float32x4_t bv = vld1q_f32(b);

    float dot = vgetq_lane_f32(av, 0) * vgetq_lane_f32(bv, 0);
    dot += vgetq_lane_f32(av, 1) * vgetq_lane_f32(bv, 1);
    dot += vgetq_lane_f32(av, 2) * vgetq_lane_f32(bv, 2);
    dot += vgetq_lane_f32(av, 3) * vgetq_lane_f32(bv, 3);

    float32x4_t delta = (dot < 0.0f) ? vsubq_f32(vnegq_f32(bv), av)
                                     : vsubq_f32(bv, av);

    float32x4_t result = vmlaq_f32(av, vdupq_n_f32(t), delta);
    vst1q_f32(&x, result);
    *this = Normalized();
}

} // namespace AbyssEngine
