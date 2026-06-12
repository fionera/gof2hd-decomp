#ifndef GOF2_AEMATH_H
#define GOF2_AEMATH_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
extern "C" float sqrtf(float);
extern "C" float sinf(float);
extern "C" float cosf(float);
extern "C" float acosf(float);
extern "C" float atanf(float);
extern "C" float powf(float, float);
extern "C" void *memcpy(void *, const void *, unsigned long);

namespace AbyssEngine {

struct Transform;

namespace AEMath {

enum RotationOrder {
    ROTATION_ORDER_XYZ = 0,
    ROTATION_ORDER_XZY = 1,
    ROTATION_ORDER_YXZ = 2,
    ROTATION_ORDER_YZX = 3,
    ROTATION_ORDER_ZXY = 4,
    ROTATION_ORDER_ZYX = 5,
};





struct BSphere {
    Vector center;
    float radius;
    float radius2;

    BSphere &operator=(const BSphere &other);
    void Merge(const ::AbyssEngine::Transform &transform);
    void Merge(const BSphere &other);
};

Vector operator+(const Vector &value);
Vector operator-(const Vector &value);
Vector operator+(const Vector &lhs, const Vector &rhs);
Vector operator-(const Vector &lhs, const Vector &rhs);
Vector operator*(const Vector &lhs, const Vector &rhs);
Vector operator*(const Vector &lhs, float rhs);
Vector operator*(float lhs, const Vector &rhs);
Vector operator/(const Vector &lhs, const Vector &rhs);
Vector operator/(const Vector &lhs, float rhs);
Vector operator/(float lhs, const Vector &rhs);
bool operator==(const Vector &lhs, const Vector &rhs);
bool operator!=(const Vector &lhs, const Vector &rhs);
bool operator<(const Vector &lhs, const Vector &rhs);
bool operator>(const Vector &lhs, const Vector &rhs);
bool operator<=(const Vector &lhs, const Vector &rhs);
bool operator>=(const Vector &lhs, const Vector &rhs);

bool VectorIsEqual(const Vector &lhs, const Vector &rhs);
float VectorDot(const Vector &lhs, const Vector &rhs);
Vector VectorCross(const Vector &lhs, const Vector &rhs);
Vector VectorNormalize(const Vector &value);
float VectorLength(const Vector &value);
Vector VectorLerp(const Vector &from, const Vector &to, float t);

Matrix operator*(const Matrix &lhs, const Matrix &rhs);
Vector operator*(const Matrix &matrix, const Vector &vector);
bool operator==(const Matrix &lhs, const Matrix &rhs);
bool operator!=(const Matrix &lhs, const Matrix &rhs);

void MatrixMultiply(Matrix &lhs, const Matrix &rhs);
Matrix MatrixIdentity(Matrix &matrix);
bool MatrixIsEqual(const Matrix &lhs, const Matrix &rhs);
Vector MatrixGetRight(const Matrix &matrix);
Vector MatrixGetUp(const Matrix &matrix);
Vector MatrixGetDir(const Matrix &matrix);
Vector MatrixGetPosition(const Matrix &matrix);
Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector);
Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector);
Vector MatrixInverseTransformVector(const Matrix &matrix, const Vector &vector);
Vector MatrixInverseRotateVector(const Matrix &matrix, const Vector &vector);
Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z);
Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z, RotationOrder order);
Matrix MatrixSetRotation(Matrix &matrix, const Vector &angles);
Matrix MatrixSetRotation(Matrix &matrix, const Vector &right, const Vector &up, const Vector &dir);
Matrix MatrixSetScaling(Matrix &matrix, float x, float y, float z);
Matrix MatrixSetTranslation(Matrix &matrix, float x, float y, float z);
Matrix MatrixSetTranslation(Matrix &matrix, const Vector &translation);
Matrix MatrixGetInverse(const Matrix &matrix);
Matrix MatrixGetLookAt(const Vector &position, const Vector &target, const Vector &up);
void MatrixGetGL(const Matrix &matrix, float *out);
void MatrixDebugOut(const Matrix &matrix);

float Sqrtf(float value);
float Sinf(float value);
float Cosf(float value);
float ACosf(float value);
float ATanf(float value);
float Absf(float value);
float Max(float lhs, float rhs);
float Min(float lhs, float rhs);
float Pow(float lhs, float rhs);
float InvSqrt(float value);

} // namespace AEMath

struct Transform {
    AEMath::Matrix matrix;
    char pad_3c[0x98];
    AEMath::BSphere bounds;

    // Out-of-line definitions live in src/Transform.cpp; declared here so the
    // mesh-loader call sites in AbyssEngine.cpp can reach them (the full class
    // with all fields/methods is in gof2/Transform.h, which can't be included
    // alongside AEMath.h's layout-only view).
    Transform();
    ~Transform();
    void CollectAnimationData();
    void SetAnimationRangeInTime(long long start, long long end);
};

static_assert(sizeof(AEMath::Vector) == 0x0c, "AEMath::Vector size");
static_assert(__builtin_offsetof(AEMath::Vector, x) == 0x00, "AEMath::Vector::x offset");
static_assert(__builtin_offsetof(AEMath::Vector, y) == 0x04, "AEMath::Vector::y offset");
static_assert(__builtin_offsetof(AEMath::Vector, z) == 0x08, "AEMath::Vector::z offset");

// NOTE: native build uses natural 64-bit AEMath::Matrix (float m[16] == 0x40),
// not the byte-matched 0x3c. Size/offset asserts that assumed 0x3c are dropped.
static_assert(__builtin_offsetof(AEMath::Matrix, m) == 0x00, "AEMath::Matrix::m offset");

static_assert(sizeof(AEMath::BSphere) == 0x14, "AEMath::BSphere size");
static_assert(__builtin_offsetof(AEMath::BSphere, center) == 0x00, "AEMath::BSphere::center offset");
static_assert(__builtin_offsetof(AEMath::BSphere, radius) == 0x0c, "AEMath::BSphere::radius offset");
static_assert(__builtin_offsetof(AEMath::BSphere, radius2) == 0x10, "AEMath::BSphere::radius2 offset");
static_assert(__builtin_offsetof(Transform, matrix) == 0x00, "Transform::matrix offset");
// Transform::bounds offset depends on natural Matrix size; byte-matched 0xd4 assert dropped.

} // namespace AbyssEngine

// NOTE: 'AEMath' is the namespace AbyssEngine::AEMath above; no separate opaque
// struct is emitted (it would clash with the namespace name).
#endif
