#include "gof2/Quaternion.h"
#include <arm_neon.h>

extern "C" float (*gof2_sinf)(float);
extern "C" float (*gof2_cosf)(float);
extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned int delta) __attribute__((noreturn));

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
    float length = x * x;
    length += y * y;
    length += z * z;
    length += w * w;
    return sqrtf(length);
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

void AbyssEngine_Quaternion_Inverse(Quaternion *result, const Quaternion *self) {
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

// ---- Set_81254.cpp ----
namespace AbyssEngine {

void Quaternion::Set(float x_angle, float y_angle, float z_angle) {
    float (*pSin)(float) = gof2_sinf;
    float fVar1 = pSin(z_angle * 0.5f);
    float fVar2 = pSin(y_angle * 0.5f);
    float (*pCos)(float) = gof2_cosf;
    float fVar3 = pSin(x_angle * 0.5f);
    float fVar4 = pCos(z_angle * 0.5f);
    float fVar5 = pCos(y_angle * 0.5f);
    float fVar6 = pCos(x_angle * 0.5f);
    x = fVar3 * fVar4 * fVar5 - fVar1 * fVar2 * fVar6;
    y = -(fVar2 * fVar4 * fVar6) + -(fVar3 * fVar1 * fVar5);
    z = fVar1 * fVar5 * fVar6 - fVar3 * fVar2 * fVar4;
    w = fVar4 * fVar5 * fVar6 + fVar1 * fVar2 * fVar3;
}

} // namespace AbyssEngine

// ---- Lerp_813a8.cpp ----
extern "C" void quaternion_sub(AbyssEngine::Quaternion *out,
                               const AbyssEngine::Quaternion *a,
                               const AbyssEngine::Quaternion *b);
extern "C" void quaternion_normalized(AbyssEngine::Quaternion *out,
                                      AbyssEngine::Quaternion *self);

namespace AbyssEngine {

void Quaternion::Lerp(const Quaternion &a, const Quaternion &b, float t) {
    typedef float Vec4 __attribute__((vector_size(16)));

    void *volatile cookie = __stack_chk_guard;
    alignas(16) unsigned char result_storage[sizeof(Quaternion)];
    alignas(16) unsigned char delta_storage[sizeof(Quaternion)];
    Quaternion *delta = reinterpret_cast<Quaternion *>(delta_storage);
    Quaternion *result = reinterpret_cast<Quaternion *>(result_storage);

    quaternion_sub(delta, &b, &a);

    float32x4_t delta_vec = *reinterpret_cast<Vec4 *>(delta);
    float32x4_t a_vec = vld1q_f32(&a.x);
    float32x4_t result_vec = vmlaq_f32(a_vec, vdupq_n_f32(t), delta_vec);
    *reinterpret_cast<Vec4 *>(result) = result_vec;
    quaternion_normalized(this, result);

    unsigned int guardDelta = (unsigned int)(__UINTPTR_TYPE__)cookie - (unsigned int)(__UINTPTR_TYPE__)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
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
extern "C" void quaternion_normalized(AbyssEngine::Quaternion *out,
                                      AbyssEngine::Quaternion *self);

namespace AbyssEngine {

void Quaternion::Lerp(const float *a, const float *b, float t) {
    typedef float Vec4 __attribute__((vector_size(16)));

    void *volatile cookie = __stack_chk_guard;

    float32x4_t bv = vld1q_f32(b);
    float32x4_t av = vld1q_f32(a);

    float dot = vgetq_lane_f32(av, 1) * vgetq_lane_f32(bv, 1);
    dot += vgetq_lane_f32(av, 0) * vgetq_lane_f32(bv, 0);
    dot += vgetq_lane_f32(av, 2) * vgetq_lane_f32(bv, 2);
    dot += vgetq_lane_f32(av, 3) * vgetq_lane_f32(bv, 3);

    float32x4_t delta;
    if (dot < 0.0f) {
        delta = vsubq_f32(vnegq_f32(bv), av);
    } else {
        delta = vsubq_f32(bv, av);
    }

    float32x4_t result = vmlaq_f32(av, vdupq_n_f32(t), delta);
    alignas(16) float storage[4];
    *reinterpret_cast<Vec4 *>(storage) = result;
    quaternion_normalized(this, reinterpret_cast<Quaternion *>(storage));

    unsigned int guardDelta = (unsigned int)(__UINTPTR_TYPE__)cookie - (unsigned int)(__UINTPTR_TYPE__)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
