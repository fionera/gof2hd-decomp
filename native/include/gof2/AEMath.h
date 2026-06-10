#ifndef GOF2_AEMATH_H
#define GOF2_AEMATH_H
#include "gof2/common.h"
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

struct Vector {
    float x;
    float y;
    float z;

    operator float *();
    operator const float *() const;
    float &operator[](int index);
    float operator[](int index) const;

    Vector &operator=(const Vector &other);
    Vector &operator+=(const Vector &other);
    Vector &operator-=(const Vector &other);
    Vector &operator*=(float scale);
    Vector &operator*=(const Vector &other);
    Vector &operator/=(float scale);
    Vector &operator/=(const Vector &other);
};

struct Matrix {
    float m[15];

    Matrix();

    operator float *();
    operator const float *() const;

    Matrix &operator=(const Matrix &other);
    Matrix &operator*=(const Matrix &other);
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
#endif
