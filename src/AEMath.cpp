#include "gof2/AEMath.h"
#include <arm_neon.h>

extern "C" void __aeabi_memcpy(void *, const void *, unsigned long);

// ---- operator_cast_to_float__7395c.cpp ----
namespace AbyssEngine {
namespace AEMath {

// Vector::operator float* defined inline in gof2/math.h

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixDebugOut_81004.cpp ----
namespace AbyssEngine {
namespace AEMath {

void MatrixDebugOut(const Matrix &matrix) {
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- Absf_86a54.cpp ----
namespace AbyssEngine {
namespace AEMath {

float Absf(float value)
{
    if (!(value >= 0.0f)) {
        value = -value;
    }
    return value;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__74144.cpp ----
namespace AbyssEngine {
namespace AEMath {

BSphere &BSphere::operator=(const BSphere &other) {
    const unsigned int *src = reinterpret_cast<const unsigned int *>(&other);
    unsigned int *dst = reinterpret_cast<unsigned int *>(this);

    uint32x4_t first = vld1q_u32(src);
    vst1q_u32(dst, first);
    dst[4] = src[4];
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator_cast_to_float__7395e.cpp ----
namespace AbyssEngine {
namespace AEMath {

// Vector::operator const float* defined inline in gof2/math.h

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixGetDir_80720.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector MatrixGetDir(const Matrix &matrix)
{
    Vector result;
    result.x = matrix.m[2];
    result.y = matrix.m[6];
    result.z = matrix.m[10];
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- ACosf_86a4c.cpp ----
namespace AbyssEngine {
namespace AEMath {

float ACosf(float value) {
    return acosf(value);
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__73bb4.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector operator/(const Vector &lhs, const Vector &rhs)
{
    Vector result;
    result.x = lhs.x / rhs.x;
    result.y = lhs.y / rhs.y;
    result.z = lhs.z / rhs.z;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixGetUp_80712.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector MatrixGetUp(const Matrix &matrix) {
    Vector result;
    result.x = matrix.m[1];
    result.y = matrix.m[5];
    result.z = matrix.m[9];
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___80448.cpp ----
namespace AbyssEngine {
namespace AEMath {

bool operator==(const Matrix &lhs, const Matrix &rhs)
{
    bool result;

    if (__builtin_expect(((((lhs.m[0] == rhs.m[0] &&
                            lhs.m[1] == rhs.m[1]) &&
                           lhs.m[2] == rhs.m[2]) &&
                          (lhs.m[3] == rhs.m[3] &&
                           lhs.m[4] == rhs.m[4])) &&
                         (lhs.m[5] == rhs.m[5] &&
                          (lhs.m[6] == rhs.m[6] &&
                           lhs.m[7] == rhs.m[7]))) &&
                        (lhs.m[8] == rhs.m[8] &&
                         lhs.m[9] == rhs.m[9]), 0)) {
        goto tail;
    }

failed:
    result = false;

done:
    return result;

tail:
    result = false;
    if (lhs.m[10] != rhs.m[10]) {
        goto done;
    }
    result = lhs.m[11] == rhs.m[11];
    goto done;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- Matrix_6e8d8.cpp ----
namespace AbyssEngine {
namespace AEMath {

// Matrix::Matrix() defined inline in gof2/math.h

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixGetGL_80fc4.cpp ----
namespace AbyssEngine {
namespace AEMath {

void MatrixGetGL(const Matrix &matrix, float *out)
{
    volatile float *dst = out;

    dst[0] = matrix.m[0];
    dst[4] = matrix.m[1];
    dst[8] = matrix.m[2];
    dst[12] = matrix.m[3];
    dst[1] = matrix.m[4];
    dst[5] = matrix.m[5];
    dst[9] = matrix.m[6];
    dst[13] = matrix.m[7];
    dst[2] = matrix.m[8];
    dst[6] = matrix.m[9];
    dst[10] = matrix.m[10];
    float z = matrix.m[11];
    dst[3] = 0.0f;
    dst[14] = z;
    dst[7] = 0.0f;
    dst[11] = 0.0f;
    dst[15] = 1.0f;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixGetPosition_8072e.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector MatrixGetPosition(const Matrix &matrix)
{
    Vector result;
    result.x = matrix.m[3];
    result.y = matrix.m[7];
    result.z = matrix.m[11];
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- Sqrtf_86a28.cpp ----
namespace AbyssEngine {
namespace AEMath {

float Sqrtf(float value)
{
    return sqrtf(value);
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__73a96.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector operator+(const Vector &value)
{
    Vector result;
    double xy = *reinterpret_cast<const double *>(&value.x);
    float z = value.z;
    result.z = z;
    *reinterpret_cast<double *>(&result.x) = xy;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixGetRight_80704.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector MatrixGetRight(const Matrix &matrix)
{
    Vector result;
    result.x = matrix.m[0];
    result.y = matrix.m[4];
    result.z = matrix.m[8];
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___73a08.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector &Vector::operator*=(const Vector &other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- Sinf_86a44.cpp ----
namespace AbyssEngine {
namespace AEMath {

float Sinf(float value) {
    return ::sinf(value);
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___73960.cpp ----
namespace AbyssEngine {
namespace AEMath {

// Vector::operator[] defined inline in gof2/math.h

} // namespace AEMath
} // namespace AbyssEngine

// ---- Pow_86aa8.cpp ----
namespace AbyssEngine {
namespace AEMath {

float Pow(float lhs, float rhs)
{
    return powf(lhs, rhs);
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- VectorDot_73dd2.cpp ----
namespace AbyssEngine {
namespace AEMath {

float VectorDot(const Vector &lhs, const Vector &rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- InvSqrt_86aac.cpp ----
namespace AbyssEngine {
namespace AEMath {

float InvSqrt(float value)
{
    static const int magic = 0x5f3759df;
    union {
        float f;
        int i;
    } bits;

    bits.f = value;
    bits.i = magic - (bits.i >> 1);
    value = value * -0.5f * bits.f * bits.f + 1.5f;
    return value * bits.f;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__73aa4.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector operator-(const Vector &value)
{
    Vector result;
    result.x = -value.x;
    result.y = -value.y;
    result.z = -value.z;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator_cast_to_float__8012e.cpp ----
namespace AbyssEngine {
namespace AEMath {

// Matrix::operator const float* defined inline in gof2/math.h

} // namespace AEMath
} // namespace AbyssEngine

// ---- VectorLerp_73eec.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector VectorLerp(const Vector &from, const Vector &to, float t)
{
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    float dz = to.z - from.z;
    Vector result;

    result.x = from.x + dx * t;
    result.y = from.y + dy * t;
    result.z = from.z + dz * t;

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- VectorLength_73eb8.cpp ----
namespace AbyssEngine {
namespace AEMath {

float VectorLength(const Vector &value)
{
    return sqrtf(value.x * value.x + value.y * value.y + value.z * value.z);
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___73966.cpp ----
namespace AbyssEngine {
namespace AEMath {

// Vector::operator[] defined inline in gof2/math.h

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__80130.cpp ----
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

// ---- operator___739ac.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector &Vector::operator-=(const Vector &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- Cosf_86a48.cpp ----
namespace AbyssEngine {
namespace AEMath {

float Cosf(float value) {
    return cosf(value);
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- VectorIsEqual_73dce.cpp ----
namespace AbyssEngine {
namespace AEMath {

bool VectorIsEqual(const Vector &lhs, const Vector &rhs) {
    return operator==(lhs, rhs);
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator_cast_to_float__8012c.cpp ----
namespace AbyssEngine {
namespace AEMath {

// Matrix::operator float* defined inline in gof2/math.h

} // namespace AEMath
} // namespace AbyssEngine

// ---- ATanf_86a50.cpp ----
namespace AbyssEngine {
namespace AEMath {

float ATanf(float value) {
    return atanf(value);
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixMultiply_80688.cpp ----
namespace AbyssEngine {
namespace AEMath {

Matrix MatrixMultiply(const Matrix &lhs, const Matrix &rhs)
{
    return operator*(lhs, rhs);
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixSetTranslation_80e48.cpp ----
namespace AbyssEngine {
namespace AEMath {

__attribute__((disable_tail_calls))
void MatrixSetTranslation(Matrix *result, Matrix &matrix, float x, float y, float z)
{
    volatile float *values = matrix.m;
    values[7] = y;
    values[3] = x;
    values[11] = z;
    __aeabi_memcpy(result, &matrix, sizeof(matrix));
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixSetTranslation_80e60.cpp ----
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;


__attribute__((disable_tail_calls)) void MatrixSetTranslation(
    Matrix *result,
    Matrix *matrix,
    const Vector *translation)
{
    matrix->m[3] = translation->x;
    matrix->m[7] = translation->y;
    matrix->m[11] = translation->z;
    __aeabi_memcpy(result, matrix, sizeof(*result));
}

// ---- operator___73c7e.cpp ----
namespace AbyssEngine {
namespace AEMath {

bool operator!=(const Vector &lhs, const Vector &rhs) {
    if (lhs.x == rhs.x) {
        if (__builtin_expect(lhs.y != rhs.y, 1)) {
            return true;
        }
        return lhs.z != rhs.z;
    }
    return true;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___8052c.cpp ----
namespace AbyssEngine {
namespace AEMath {

bool operator!=(const Matrix &lhs, const Matrix &rhs)
{
    bool result;

    if (__builtin_expect((((((lhs.m[0] == rhs.m[0] &&
                             lhs.m[1] == rhs.m[1]) &&
                            lhs.m[2] == rhs.m[2]) &&
                           (lhs.m[3] == rhs.m[3] &&
                            lhs.m[4] == rhs.m[4])) &&
                          (lhs.m[5] == rhs.m[5] &&
                           (lhs.m[6] == rhs.m[6] &&
                            lhs.m[7] == rhs.m[7]))) &&
                         (lhs.m[8] == rhs.m[8] &&
                          (lhs.m[9] == rhs.m[9] &&
                           lhs.m[10] == rhs.m[10]))), 0)) {
        goto tail;
    }

    result = true;

done:
    return result;

tail:
    result = lhs.m[11] != rhs.m[11];
    goto done;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- Max_86a70.cpp ----
namespace AbyssEngine {
namespace AEMath {

float Max(float lhs, float rhs)
{
    if (lhs > rhs) {
        rhs = lhs;
    }
    return rhs;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixGetInverse_80e78.cpp ----
namespace AbyssEngine {
namespace AEMath {

Matrix MatrixGetInverse(const Matrix &matrix)
{
    Matrix result;

    result.m[0] = matrix.m[0];
    result.m[1] = matrix.m[4];
    result.m[2] = matrix.m[8];

    result.m[4] = matrix.m[1];
    result.m[5] = matrix.m[5];
    result.m[6] = matrix.m[9];

    result.m[8] = matrix.m[2];
    result.m[9] = matrix.m[6];
    result.m[10] = matrix.m[10];

    float x = -(matrix.m[3] * matrix.m[0]) - (matrix.m[7] * matrix.m[4]) - (matrix.m[11] * matrix.m[8]);
    float y = -(matrix.m[3] * matrix.m[1]) - (matrix.m[7] * matrix.m[5]) - (matrix.m[11] * matrix.m[9]);
    float z = -(matrix.m[3] * matrix.m[2]) - (matrix.m[7] * matrix.m[6]) - (matrix.m[11] * matrix.m[10]);

    result.m[3] = x;
    result.m[7] = y;
    result.m[11] = z;

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___73a64.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector &Vector::operator/=(const Vector &other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__73c10.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector operator/(float lhs, const Vector &rhs)
{
    float x = rhs.x;
    float y = rhs.y;
    float z = rhs.z;
    float scale = lhs;
    Vector result;
    result.x = x / scale;
    result.y = y / scale;
    result.z = z / scale;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixIsEqual_80700.cpp ----
namespace AbyssEngine {
namespace AEMath {

bool MatrixIsEqual(const Matrix &lhs, const Matrix &rhs)
{
    return lhs == rhs;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixSetRotation_80d40.cpp ----
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;


__attribute__((disable_tail_calls)) void MatrixSetRotation(
    Matrix *result,
    Matrix *matrix,
    const Vector *right,
    const Vector *up,
    const Vector *dir)
{
    matrix->m[0] = right->x;
    matrix->m[4] = right->y;
    matrix->m[8] = right->z;
    matrix->m[1] = up->x;
    matrix->m[5] = up->y;
    matrix->m[9] = up->z;
    matrix->m[2] = dir->x;
    matrix->m[6] = dir->y;
    matrix->m[10] = dir->z;
    __aeabi_memcpy(result, matrix, sizeof(*matrix));
}

// ---- operator__7396c.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector &Vector::operator=(const Vector &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__73b60.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector operator*(const Vector &lhs, float rhs)
{
    float x = lhs.x;
    float y = lhs.y;
    float z = lhs.z;
    Vector result;

    result.x = x * rhs;
    result.y = y * rhs;
    result.z = z * rhs;

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__73afc.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector operator-(const Vector &lhs, const Vector &rhs)
{
    Vector result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- Min_86a8c.cpp ----
namespace AbyssEngine {
namespace AEMath {

float Min(float lhs, float rhs)
{
    return lhs < rhs ? lhs : rhs;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixIdentity_80694.cpp ----
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

extern "C" __attribute__((disable_tail_calls)) void MatrixIdentity(Matrix *result,
                                                                   Matrix *matrix)
{
    float32x4_t zero = vdupq_n_f32(0.0f);

    vst1q_f32(&matrix->m[1], zero);
    vst1q_f32(&matrix->m[6], zero);

    volatile unsigned int *out = reinterpret_cast<volatile unsigned int *>(matrix->m);
    unsigned int one = 0x3f800000;
    out[0] = one;
    out[5] = one;
    matrix->m[10] = 1.0f;
    matrix->m[11] = 0.0f;

    Vector diagonal;
    diagonal.x = 1.0f;
    diagonal.y = 1.0f;
    diagonal.z = 1.0f;
    *reinterpret_cast<Vector *>(&matrix->m[12]) = diagonal;

    __aeabi_memcpy(result, matrix, sizeof(*result));
    return;
}

// ---- MatrixGetLookAt_80f18.cpp ----
namespace AbyssEngine {
namespace AEMath {

void MatrixGetLookAt(Matrix &result, const Vector &position, const Vector &target, const Vector &up)
{
    Vector diff = position - target;
    Vector dir = VectorNormalize(diff);
    Vector cross = VectorCross(up, dir);
    Vector right = VectorNormalize(cross);
    cross = VectorCross(dir, right);

    result.m[0] = right.x;
    result.m[1] = cross.x;
    result.m[2] = dir.x;
    result.m[3] = position.x;
    result.m[4] = right.y;
    result.m[5] = cross.y;
    result.m[6] = dir.y;
    result.m[7] = position.y;
    result.m[8] = right.z;
    result.m[9] = cross.z;
    result.m[10] = dir.z;
    result.m[11] = position.z;
    result.m[12] = 1.0f;
    result.m[13] = 1.0f;
    result.m[14] = 1.0f;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- Merge_74000.cpp ----
namespace AbyssEngine {
namespace AEMath {

void BSphere::Merge(const BSphere &other)
{
    if (other.radius2 > radius2) {
        radius2 = other.radius2;
    }

    if (other.radius != 0.0f) {
        if (radius == 0.0f) {
            radius = other.radius;
            __builtin_memcpy(&center.x, &other.center.x, sizeof(center.x));
            __builtin_memcpy(&center.y, &other.center.y, sizeof(center.y));
            __builtin_memcpy(&center.z, &other.center.z, sizeof(center.z));
            __builtin_memcpy(&radius2, &other.radius2, sizeof(radius2));
            operator=(other);
        } else {
            float dx = other.center.x - center.x;
            float dy = other.center.y - center.y;
            float dz = other.center.z - center.z;
            float distance2 = dx * dx + dy * dy + dz * dz;
            float distance = sqrtf(distance2);

            float otherRadius = other.radius;
            float thisRadius = radius;
            if (distance == 0.0f) {
                if (thisRadius > otherRadius) {
                    otherRadius = thisRadius;
                }
            } else {
                float distancePlusOther = distance + otherRadius;
                if (thisRadius > distancePlusOther) {
                    return;
                }
                if (distance - otherRadius < -thisRadius) {
                    __builtin_memcpy(&center.x, &other.center.x, sizeof(center.x));
                    __builtin_memcpy(&center.y, &other.center.y, sizeof(center.y));
                    __builtin_memcpy(&center.z, &other.center.z, sizeof(center.z));
                    radius = otherRadius;
                    return;
                }

                float scale = ((distancePlusOther - thisRadius) * 0.5f) / distance;
                center.y = center.y + dy * scale;
                center.z = center.z + dz * scale;
                otherRadius = (distance + thisRadius + otherRadius) * 0.5f;
                center.x = center.x + dx * scale;
            }
            radius = otherRadius;
        }
    }
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__73b8a.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector operator*(float lhs, const Vector &rhs)
{
    float x = rhs.x;
    float y = rhs.y;
    float z = rhs.z;
    Vector result;
    result.x = x * lhs;
    result.y = y * lhs;
    result.z = z * lhs;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixRotateVector_807b6.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector)
{
    Vector result;
    result.y = vector.y * matrix.m[5];
    result.x = vector.y * matrix.m[1];
    result.z = vector.y * matrix.m[9];
    result.y += vector.x * matrix.m[4];
    result.x += vector.x * matrix.m[0];
    result.z += vector.x * matrix.m[8];
    result.y += vector.z * matrix.m[6];
    result.x += vector.z * matrix.m[2];
    result.z += vector.z * matrix.m[10];
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___739de.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector &Vector::operator*=(float scale) {
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__8060e.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector operator*(const Matrix &matrix, const Vector &vector)
{
    Vector result;

    const volatile float *v = &vector.y;
    const volatile float *mx = &matrix.m[1];
    const volatile float *my = &matrix.m[5];
    const volatile float *mz = &matrix.m[9];

    float vy = v[0];
    float m1 = mx[0];
    float m5 = my[0];
    float m9 = mz[0];

    float x = vy * m1;
    float y = vy * m5;
    float z = vy * m9;

    float vx = v[-1];
    float m0 = mx[-1];
    float m4 = my[-1];
    float m8 = mz[-1];

    x += vx * m0;
    y += vx * m4;
    z += vx * m8;

    float vz = v[1];
    float m2 = mx[1];
    float m6 = my[1];
    float m10 = mz[1];
    float m3 = mx[2];
    float m7 = my[2];
    float m11 = mz[2];

    x += vz * m2;
    y += vz * m6;
    z += vz * m10;

    result.x = m3 + x;
    result.y = m7 + y;
    result.z = m11 + z;

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixInverseRotateVector_808b6.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector MatrixInverseRotateVector(const Matrix &matrix, const Vector &vector)
{
    Vector result;
    const volatile float *v = &vector.x;

    float y = v[1];

    float resultX = y * matrix.m[4];
    float resultY = y * matrix.m[5];
    float resultZ = y * matrix.m[6];

    float x = v[0];

    resultX = x * matrix.m[0] + resultX;
    resultY = x * matrix.m[1] + resultY;
    resultZ = x * matrix.m[2] + resultZ;

    float z = v[2];

    resultX = z * matrix.m[8] + resultX;
    resultY = z * matrix.m[9] + resultY;
    resultZ = z * matrix.m[10] + resultZ;

    result.x = resultX;
    result.y = resultY;
    result.z = resultZ;

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__73be6.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector operator/(const Vector &lhs, float rhs)
{
    float x = lhs.x;
    float y = lhs.y;
    float z = lhs.z;
    Vector result;
    result.x = x / rhs;
    result.y = y / rhs;
    result.z = z / rhs;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixSetRotation_809f0.cpp ----
extern "C" float (*MatrixSetRotation_809f0_sinf)(float) = ::sinf;
extern "C" float (*MatrixSetRotation_809f0_cosf)(float) = ::cosf;

namespace AbyssEngine {
namespace AEMath {

Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z, RotationOrder order)
{
    float (*sinFunc)(float) = MatrixSetRotation_809f0_sinf;
    float sx = sinFunc(x);
    float (*cosFunc)(float) = MatrixSetRotation_809f0_cosf;
    float sy = sinFunc(y);
    float sz = sinFunc(z);
    float cx = cosFunc(x);
    float cy = cosFunc(y);
    float cz = cosFunc(z);

    switch (order) {
    case ROTATION_ORDER_XYZ:
        matrix.m[0] = cy * cz;
        matrix.m[1] = -(sz * cy);
        matrix.m[2] = sy;
        matrix.m[8] = sx * sz - sy * cx * cz;
        matrix.m[9] = sx * cz + sz * sy * cx;
        matrix.m[10] = cx * cy;
        matrix.m[4] = sy * sx * cz + sz * cx;
        matrix.m[5] = cx * cz - sx * sy * sz;
        matrix.m[6] = -(sx * cy);
        break;

    case ROTATION_ORDER_XZY:
        matrix.m[0] = cy * cz;
        matrix.m[1] = -sz;
        matrix.m[2] = sy * cz;
        matrix.m[4] = sx * sy + sz * cx * cy;
        matrix.m[5] = cx * cz;
        matrix.m[6] = sz * sy * cx - sx * cy;
        matrix.m[8] = sz * sx * cy - sy * cx;
        matrix.m[9] = sx * cz;
        matrix.m[10] = cx * cy + sx * sy * sz;
        break;

    case ROTATION_ORDER_YXZ:
        matrix.m[4] = sz * cx;
        matrix.m[5] = cx * cz;
        matrix.m[6] = -sx;
        matrix.m[0] = cy * cz + sx * sy * sz;
        matrix.m[1] = sy * sx * cz - sz * cy;
        matrix.m[2] = sy * cx;
        matrix.m[8] = sz * sx * cy - sy * cz;
        matrix.m[9] = sy * sz + sx * cy * cz;
        matrix.m[10] = cx * cy;
        break;

    case ROTATION_ORDER_YZX:
        matrix.m[4] = sz;
        matrix.m[5] = cx * cz;
        matrix.m[6] = -(sx * cz);
        matrix.m[8] = -(sy * cz);
        matrix.m[9] = sx * cy + cx * sy * sz;
        matrix.m[10] = cx * cy - sx * sy * sz;
        matrix.m[0] = cy * cz;
        matrix.m[1] = sx * sy - cx * sz * cy;
        matrix.m[2] = sx * sz * cy + sy * cx;
        break;

    case ROTATION_ORDER_ZXY:
        matrix.m[8] = -(sy * cx);
        matrix.m[9] = sx;
        matrix.m[10] = cx * cy;
        matrix.m[0] = cy * cz - sx * sy * sz;
        matrix.m[1] = -(sz * cx);
        matrix.m[2] = sy * cz + sx * sz * cy;
        matrix.m[4] = sy * sx * cz + sz * cy;
        matrix.m[5] = cx * cz;
        matrix.m[6] = sy * sz - sx * cy * cz;
        break;

    case ROTATION_ORDER_ZYX:
        matrix.m[8] = -sy;
        matrix.m[9] = sx * cy;
        matrix.m[10] = cx * cy;
        matrix.m[4] = sz * cy;
        matrix.m[5] = cx * cz + sx * sy * sz;
        matrix.m[6] = cx * sy * sz - sx * cz;
        matrix.m[0] = cy * cz;
        matrix.m[1] = sy * sx * cz - sz * cx;
        matrix.m[2] = sy * cx * cz + sx * sz;
        break;
    }

    return matrix;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__73cbe.cpp ----
namespace AbyssEngine {
namespace AEMath {

bool operator<(const Vector &lhs, const Vector &rhs)
{
    if (!(lhs.x < rhs.x)) {
        return false;
    }
    if (!(lhs.y < rhs.y)) {
        return false;
    }
    return lhs.z < rhs.z;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__73aca.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector operator+(const Vector &lhs, const Vector &rhs)
{
    Vector result;

    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__802c8.cpp ----
namespace AbyssEngine {
namespace AEMath {


// Matrix::Matrix() defined inline in gof2/math.h

Matrix operator*(const Matrix &lhs, const Matrix &rhs)
{
    Vector scale;
    unsigned int one = 0x3f800000;
    Matrix result;

    float a9 = lhs.m[9];
    float b6 = rhs.m[6];
    float a1 = lhs.m[1];
    float a5 = lhs.m[5];
    float r10 = b6 * a9;
    float r2 = a1 * b6;
    float b5 = rhs.m[5];
    float r6 = b6 * a5;
    float b7 = rhs.m[7];
    float b4 = rhs.m[4];
    float r1 = a1 * b5;
    float b2 = rhs.m[2];
    float r11 = b7 * a9;
    float a8 = lhs.m[8];
    float r4 = b4 * a5;
    float a4 = lhs.m[4];
    float r9 = b5 * a9;
    float a0 = lhs.m[0];
    r10 += b2 * a8;
    r6 += b2 * a4;
    float b3 = rhs.m[3];
    r2 += a0 * b2;
    float a10 = lhs.m[10];
    float r5 = b5 * a5;
    float a6 = lhs.m[6];
    float r7 = b7 * a5;
    float b11 = rhs.m[11];
    float r3 = a1 * b7;
    float a2 = lhs.m[2];
    r11 += b3 * a8;
    float b1 = rhs.m[1];
    float r8 = b4 * a9;
    float b0 = rhs.m[0];
    float r0 = a1 * b4;
    float b10 = rhs.m[10];
    float r4v = b4 * a4;
    float b8 = rhs.m[8];
    r7 += b3 * a4;
    float b9 = rhs.m[9];
    r3 += a0 * b3;
    float a3 = lhs.m[3];
    r11 += b11 * a10;
    r1 += a0 * b1;
    r9 += b1 * a8;
    r5 += b1 * a4;
    float a7 = lhs.m[7];
    r7 += b11 * a6;
    r3 += a2 * b11;
    r8 += b0 * a8;
    r0 += a0 * b0;
    float a11 = lhs.m[11];
    r10 += b10 * a10;
    r4v += b0 * a4;
    *reinterpret_cast<unsigned int *>(&result.m[13]) = one;
    *reinterpret_cast<unsigned int *>(&result.m[14]) = one;
    r2 += a2 * b10;
    r1 += a2 * b9;
    r5 += b9 * a6;
    r0 += a2 * b8;
    r6 += b10 * a6;
    r8 += b8 * a10;
    r9 += b9 * a10;

    result.m[0] = r0;
    result.m[1] = r1;
    result.m[2] = r2;
    result.m[3] = a3 + r3;
    result.m[4] = r4v;
    result.m[5] = r5;
    result.m[6] = r6;
    result.m[7] = a7 + r7;
    result.m[8] = r8;
    result.m[9] = r9;
    result.m[10] = r10;
    result.m[11] = a11 + r11;

    *reinterpret_cast<unsigned int *>(&result.m[12]) = one;
    scale = reinterpret_cast<const Vector &>(lhs.m[12]) * reinterpret_cast<const Vector &>(rhs.m[12]);
    reinterpret_cast<Vector &>(result.m[12]) = scale;

    

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixSetScaling_80d84.cpp ----
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;


extern "C" __attribute__((stack_protect_req, disable_tail_calls)) void MatrixSetScaling(
    Matrix *result,
    Matrix *matrix,
    float x,
    float y,
    float z)
{
    matrix->m[0] *= x;
    matrix->m[4] *= x;
    matrix->m[8] *= x;
    matrix->m[1] *= y;
    matrix->m[5] *= y;
    matrix->m[9] *= y;
    matrix->m[2] *= z;
    matrix->m[6] *= z;
    matrix->m[10] *= z;

    Vector scale;
    scale.x = x;
    scale.y = y;
    scale.z = z;
    *reinterpret_cast<Vector *>(&matrix->m[12]) = scale;

    __aeabi_memcpy(result, matrix, sizeof(*matrix));
}

// ---- VectorCross_73dfc.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector VectorCross(const Vector &lhs, const Vector &rhs) {
    Vector result;
    float y = lhs.z * rhs.x;
    float z = rhs.y * lhs.x;
    float x = lhs.y * rhs.z;
    y -= lhs.x * rhs.z;
    z -= rhs.x * lhs.y;
    x -= lhs.z * rhs.y;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___73c3a.cpp ----
namespace AbyssEngine {
namespace AEMath {

bool operator==(const Vector &lhs, const Vector &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__73b2e.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector operator*(const Vector &lhs, const Vector &rhs)
{
    Vector result;
    result.x = lhs.x * rhs.x;
    result.y = lhs.y * rhs.y;
    result.z = lhs.z * rhs.z;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___73d8a.cpp ----
namespace AbyssEngine {
namespace AEMath {

bool operator>=(const Vector &lhs, const Vector &rhs)
{
    if (!(lhs.x >= rhs.x)) {
        return false;
    }
    bool result = false;
    if (!(lhs.y >= rhs.y)) {
        return result;
    }
    if (lhs.z >= rhs.z) {
        result = true;
    }
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___73d46.cpp ----
namespace AbyssEngine {
namespace AEMath {

bool operator<=(const Vector &lhs, const Vector &rhs)
{
    return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___8016a.cpp ----
namespace AbyssEngine {
namespace AEMath {

Matrix &Matrix::operator*=(const Matrix &other) {
    float y0 = m[1];
    float b10 = other.m[4];
    float x0 = m[0];
    float result = y0 * b10;
    float b00 = other.m[0];
    float z0 = m[2];
    float b20 = other.m[8];
    float w0 = m[3];
    result += x0 * b00;
    result += z0 * b20;
    m[0] = result;

    float b11 = other.m[5];
    float b01 = other.m[1];
    result = y0 * b11;
    float b21 = other.m[9];
    result += x0 * b01;
    result += z0 * b21;
    m[1] = result;

    float b12 = other.m[6];
    float b02 = other.m[2];
    result = y0 * b12;
    float b22 = other.m[10];
    result += x0 * b02;
    result += z0 * b22;
    m[2] = result;

    float b13 = other.m[7];
    float b03 = other.m[3];
    result = y0 * b13;
    float b23 = other.m[11];
    result += x0 * b03;
    result += z0 * b23;
    m[3] = w0 + result;

    float x1 = m[4];
    b00 = other.m[0];
    float y1 = m[5];
    result = x1 * b00;
    float z1 = m[6];
    result += b10 * y1;
    result += b20 * z1;
    m[4] = result;

    b01 = other.m[1];
    result = x1 * b01;
    result += b11 * y1;
    result += b21 * z1;
    m[5] = result;

    b02 = other.m[2];
    result = x1 * b02;
    result += b12 * y1;
    result += b22 * z1;
    m[6] = result;

    b03 = other.m[3];
    result = x1 * b03;
    result += b13 * y1;
    w0 = m[7];
    result += b23 * z1;
    m[7] = w0 + result;

    float y2 = m[9];
    b10 = other.m[4];
    float x2 = m[8];
    result = y2 * b10;
    float z2 = m[10];
    result += b00 * x2;
    result += b20 * z2;
    m[8] = result;

    b11 = other.m[5];
    result = y2 * b11;
    result += b01 * x2;
    result += b21 * z2;
    m[9] = result;

    b12 = other.m[6];
    result = y2 * b12;
    result += b02 * x2;
    result += b22 * z2;
    m[10] = result;

    b13 = other.m[7];
    result = y2 * b13;
    result += b03 * x2;
    result += b23 * z2;
    m[11] += result;

    reinterpret_cast<Vector *>(&m[12])->operator*=(*reinterpret_cast<const Vector *>(&other.m[12]));
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___73a3a.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector &Vector::operator/=(float scale)
{
    x /= scale;
    y /= scale;
    z /= scale;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__73d02.cpp ----
namespace AbyssEngine {
namespace AEMath {

bool operator>(const Vector &lhs, const Vector &rhs)
{
    bool result = false;
    if (lhs.x > rhs.x) {
        if (lhs.y > rhs.y) {
            result = lhs.z > rhs.z;
        }
    }
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixSetRotation_80918.cpp ----
namespace AbyssEngine {
namespace AEMath {

Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z)
{
    float (*sin_ptr)(float) = ::sinf;
    float sx = sin_ptr(x);
    float (*cos_ptr)(float) = ::cosf;
    float sy = sin_ptr(y);
    float sz = sin_ptr(z);
    float cx = cos_ptr(x);
    float cy = cos_ptr(y);
    float cz = cos_ptr(z);

    matrix.m[0] = cy * cz;
    matrix.m[1] = -(sz * cy);
    matrix.m[2] = sy;
    matrix.m[8] = sx * sz - sy * (cx * cz);
    matrix.m[9] = sx * cz + sz * (sy * cx);
    matrix.m[10] = cx * cy;
    matrix.m[4] = sy * (sx * cz) + sz * cx;
    matrix.m[5] = cx * cz - (sx * sy) * sz;
    matrix.m[6] = -(sx * cy);
    return matrix;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator___7397a.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector &Vector::operator+=(const Vector &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixInverseTransformVector_80818.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector MatrixInverseTransformVector(const Matrix &matrix, const Vector &vector)
{
    Vector result;

    float xBase = -(matrix.m[4] * matrix.m[7]) - matrix.m[0] * matrix.m[3];
    float yBase = -(matrix.m[7] * matrix.m[5]) - matrix.m[3] * matrix.m[1];
    float zBase = -(matrix.m[7] * matrix.m[6]) - matrix.m[3] * matrix.m[2];

    xBase -= matrix.m[8] * matrix.m[11];
    yBase -= matrix.m[11] * matrix.m[9];
    zBase -= matrix.m[11] * matrix.m[10];

    result.x = vector.y * matrix.m[4] + vector.x * matrix.m[0] + vector.z * matrix.m[8] + xBase;
    result.y = vector.y * matrix.m[5] + vector.x * matrix.m[1] + vector.z * matrix.m[9] + yBase;
    result.z = vector.y * matrix.m[6] + vector.x * matrix.m[2] + vector.z * matrix.m[10] + zBase;

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- Merge_73f30.cpp ----
namespace AbyssEngine {
namespace AEMath {

void BSphere::Merge(const ::AbyssEngine::Transform &transform) {

    BSphere other;

    other.radius2 = 1.0f;
    Vector temp = MatrixTransformVector(transform.matrix, transform.bounds.center);
    other.center.x = temp.x;
    other.center.y = temp.y;
    other.center.z = temp.z;

    temp.x = transform.bounds.radius;
    temp.y = transform.bounds.radius;
    temp.z = transform.bounds.radius;
    Vector rotated = MatrixRotateVector(transform.matrix, temp);

    float abs_x = -rotated.x;
    if (0.0f < rotated.x) {
        abs_x = rotated.x;
    }

    float abs_y = -rotated.y;
    if (0.0f < rotated.y) {
        abs_y = rotated.y;
    }

    float abs_z = -rotated.z;
    if (0.0f < rotated.z) {
        abs_z = rotated.z;
    }

    if (abs_y < abs_x) {
        abs_y = abs_x;
    }
    if (abs_z < abs_y) {
        abs_z = abs_y;
    }

    other.radius = abs_z;
    Merge(other);

    return;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixSetRotation_80cc4.cpp ----
namespace AbyssEngine {
namespace AEMath {

void VectorCross(Vector *result, const Vector &lhs, const Vector &rhs);
void VectorNormalize(Vector *result, const Vector &value);
void MatrixSetRotation(Matrix *result, Matrix &matrix, const Vector &right, const Vector &up, const Vector &dir);

void MatrixSetRotation(Matrix *result, Matrix &matrix, const Vector &dir)
{
    Vector right;
    Vector adjustedUp;
    Vector work;

    work.x = 0.0f;
    work.y = 1.0f;
    work.z = 0.0f;

    VectorCross(&adjustedUp, work, dir);
    VectorNormalize(&right, adjustedUp);
    VectorCross(&work, dir, right);
    VectorNormalize(&adjustedUp, work);
    MatrixSetRotation(result, matrix, right, adjustedUp, dir);

    return;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- VectorNormalize_73e3c.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector VectorNormalize(const Vector &value) {
    const float length = sqrtf(value.x * value.x + value.y * value.y + value.z * value.z);
    Vector result;
    if (length == 0.0f) {
        result.x = 0.0f;
        result.y = 1.0f;
        result.z = 0.0f;
    } else {
        result.x = value.x / length;
        result.y = value.y / length;
        result.z = value.z / length;
    }
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- MatrixTransformVector_8073c.cpp ----
namespace AbyssEngine {
namespace AEMath {

Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector)
{
    const float *m = matrix.m;

    float vy = vector.y;
    float m1 = m[1];
    float m5 = m[5];
    float m9 = m[9];
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    float x = vy * m1;
    float y = vy * m5;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    float vx = vector.x;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    float z = vy * m9;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    float m0 = m[0];
    float m4 = m[4];
    float m8 = m[8];
    float vz = vector.z;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    x += vx * m0;
    float m2 = m[2];
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    y += vx * m4;
    float m6 = m[6];
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    z += vx * m8;
    float m10 = m[10];
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    float tx = m[3];
    float ty = m[7];
    float tz = m[11];
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    x += vz * m2;
    y += vz * m6;
    z += vz * m10;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    Vector result;
    result.x = tx + x;
    result.y = ty + y;
    result.z = tz + z;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
