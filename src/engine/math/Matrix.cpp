#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"
#include "engine/math/Quaternion.h"
#include "engine/math/AEMath.h"

#include <cmath>
#include <cstring>

// Original Matrix.cpp TU (Android .text 0x8012d-0x81005, followed by Quaternion): Matrix operators
// and the Matrix* helpers.
namespace AbyssEngine {
    namespace AEMath {
        Matrix::operator float *() { return m; }
        Matrix::operator const float *() const { return m; }

        Matrix &Matrix::operator=(const Matrix &other) {
            m.e0 = other.m.e0;
            m.e1 = other.m.e1;
            m.e2 = other.m.e2;
            m.e3 = other.m.e3;
            m.e4 = other.m.e4;
            m.e5 = other.m.e5;
            m.e6 = other.m.e6;
            m.e7 = other.m.e7;
            m.e8 = other.m.e8;
            m.e9 = other.m.e9;
            m.e10 = other.m.e10;
            m.e11 = other.m.e11;
            scaleVector() = other.scaleVector();
            return *this;
        }

        bool operator==(const Matrix &lhs, const Matrix &rhs) {
            return !(lhs.m[0] != rhs.m[0] ||
                     lhs.m[1] != rhs.m[1] ||
                     lhs.m[2] != rhs.m[2] ||
                     lhs.m[3] != rhs.m[3] ||
                     lhs.m[4] != rhs.m[4] ||
                     lhs.m[5] != rhs.m[5] ||
                     lhs.m[6] != rhs.m[6] ||
                     lhs.m[7] != rhs.m[7] ||
                     lhs.m[8] != rhs.m[8] ||
                     lhs.m[9] != rhs.m[9] ||
                     lhs.m[10] != rhs.m[10] ||
                     lhs.m[11] != rhs.m[11]);
        }

        bool operator!=(const Matrix &lhs, const Matrix &rhs) {
            return lhs.m[0] != rhs.m[0] ||
                   lhs.m[1] != rhs.m[1] ||
                   lhs.m[2] != rhs.m[2] ||
                   lhs.m[3] != rhs.m[3] ||
                   lhs.m[4] != rhs.m[4] ||
                   lhs.m[5] != rhs.m[5] ||
                   lhs.m[6] != rhs.m[6] ||
                   lhs.m[7] != rhs.m[7] ||
                   lhs.m[8] != rhs.m[8] ||
                   lhs.m[9] != rhs.m[9] ||
                   lhs.m[10] != rhs.m[10] ||
                   lhs.m[11] != rhs.m[11];
        }

        Matrix operator*(const Matrix &lhs, const Matrix &rhs) {
            const float m0 = lhs.m[0] * rhs.m[0] + lhs.m[1] * rhs.m[4] + lhs.m[2] * rhs.m[8];
            const float m1 = lhs.m[0] * rhs.m[1] + lhs.m[1] * rhs.m[5] + lhs.m[2] * rhs.m[9];
            const float m2 = lhs.m[0] * rhs.m[2] + lhs.m[1] * rhs.m[6] + lhs.m[2] * rhs.m[10];
            const float m3 = lhs.m[3] + (lhs.m[0] * rhs.m[3] + lhs.m[1] * rhs.m[7] + lhs.m[2] * rhs.m[11]);
            const float m4 = lhs.m[4] * rhs.m[0] + lhs.m[5] * rhs.m[4] + lhs.m[6] * rhs.m[8];
            const float m5 = lhs.m[4] * rhs.m[1] + lhs.m[5] * rhs.m[5] + lhs.m[6] * rhs.m[9];
            const float m6 = lhs.m[4] * rhs.m[2] + lhs.m[5] * rhs.m[6] + lhs.m[6] * rhs.m[10];
            const float m7 = lhs.m[7] + (lhs.m[4] * rhs.m[3] + lhs.m[5] * rhs.m[7] + lhs.m[6] * rhs.m[11]);
            const float m8 = lhs.m[8] * rhs.m[0] + lhs.m[9] * rhs.m[4] + lhs.m[10] * rhs.m[8];
            const float m9 = lhs.m[8] * rhs.m[1] + lhs.m[9] * rhs.m[5] + lhs.m[10] * rhs.m[9];
            const float m10 = lhs.m[8] * rhs.m[2] + lhs.m[9] * rhs.m[6] + lhs.m[10] * rhs.m[10];
            const float m11 = lhs.m[11] + (lhs.m[8] * rhs.m[3] + lhs.m[9] * rhs.m[7] + lhs.m[10] * rhs.m[11]);
            Matrix result(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11);
            result.scaleVector() = lhs.scaleVector() * rhs.scaleVector();
            return result;
        }

        Vector operator*(const Matrix &matrix, const Vector &vector) {
            const float x = vector.x * matrix.m[0] + vector.y * matrix.m[1] + vector.z * matrix.m[2] + matrix.m[3];
            const float y = vector.x * matrix.m[4] + vector.y * matrix.m[5] + vector.z * matrix.m[6] + matrix.m[7];
            const float z = vector.x * matrix.m[8] + vector.y * matrix.m[9] + vector.z * matrix.m[10] + matrix.m[11];
            Vector result;
            result.x = x;
            result.y = y;
            result.z = z;
            return result;
        }

        Matrix &Matrix::operator*=(const Matrix &other) {
            const MatrixElements &o = other.m;
            float r0 = m.e0, r1 = m.e1, r2 = m.e2;
            m.e0 = r0 * o.e0 + r1 * o.e4 + r2 * o.e8;
            m.e1 = r0 * o.e1 + r1 * o.e5 + r2 * o.e9;
            m.e2 = r0 * o.e2 + r1 * o.e6 + r2 * o.e10;
            m.e3 = m.e3 + (r0 * o.e3 + r1 * o.e7 + r2 * o.e11);
            r0 = m.e4;
            r1 = m.e5;
            r2 = m.e6;
            m.e4 = r0 * o.e0 + r1 * o.e4 + r2 * o.e8;
            m.e5 = r0 * o.e1 + r1 * o.e5 + r2 * o.e9;
            m.e6 = r0 * o.e2 + r1 * o.e6 + r2 * o.e10;
            m.e7 = m.e7 + (r0 * o.e3 + r1 * o.e7 + r2 * o.e11);
            r0 = m.e8;
            r1 = m.e9;
            r2 = m.e10;
            m.e8 = r0 * o.e0 + r1 * o.e4 + r2 * o.e8;
            m.e9 = r0 * o.e1 + r1 * o.e5 + r2 * o.e9;
            m.e10 = r0 * o.e2 + r1 * o.e6 + r2 * o.e10;
            m.e11 = m.e11 + (r0 * o.e3 + r1 * o.e7 + r2 * o.e11);
            scaleVector() *= other.scaleVector();
            return *this;
        }

        Matrix MatrixMultiply(const Matrix &lhs, const Matrix &rhs) {
            return lhs * rhs;
        }

        Matrix MatrixIdentity(Matrix &matrix) {
            matrix.m[0] = 1.0f;
            matrix.m[1] = 0.0f;
            matrix.m[2] = 0.0f;
            matrix.m[3] = 0.0f;
            matrix.m[4] = 0.0f;
            matrix.m[5] = 1.0f;
            matrix.m[6] = 0.0f;
            matrix.m[7] = 0.0f;
            matrix.m[8] = 0.0f;
            matrix.m[9] = 0.0f;
            matrix.m[10] = 1.0f;
            matrix.m[11] = 0.0f;
            matrix.scaleVector() = Vector{1.0f, 1.0f, 1.0f};
            return matrix;
        }

        bool MatrixIsEqual(const Matrix &lhs, const Matrix &rhs) {
            return lhs == rhs;
        }

        Vector MatrixGetRight(const Matrix &matrix) {
            Vector result;
            result.x = matrix.m[0];
            result.y = matrix.m[4];
            result.z = matrix.m[8];
            return result;
        }

        Vector MatrixGetUp(const Matrix &matrix) {
            Vector result;
            result.x = matrix.m[1];
            result.y = matrix.m[5];
            result.z = matrix.m[9];
            return result;
        }

        Vector MatrixGetDir(const Matrix &matrix) {
            Vector result;
            result.x = matrix.m[2];
            result.y = matrix.m[6];
            result.z = matrix.m[10];
            return result;
        }

        Vector MatrixGetPosition(const Matrix &matrix) {
            Vector result;
            result.x = matrix.m[3];
            result.y = matrix.m[7];
            result.z = matrix.m[11];
            return result;
        }

        Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector) {
            const float x = vector.x * matrix.m[0] + vector.y * matrix.m[1] + vector.z * matrix.m[2] + matrix.m[3];
            const float y = vector.x * matrix.m[4] + vector.y * matrix.m[5] + vector.z * matrix.m[6] + matrix.m[7];
            const float z = vector.x * matrix.m[8] + vector.y * matrix.m[9] + vector.z * matrix.m[10] + matrix.m[11];
            Vector result;
            result.x = x;
            result.y = y;
            result.z = z;
            return result;
        }

        Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector) {
            const float x = vector.x * matrix.m[0] + vector.y * matrix.m[1] + vector.z * matrix.m[2];
            const float y = vector.x * matrix.m[4] + vector.y * matrix.m[5] + vector.z * matrix.m[6];
            const float z = vector.x * matrix.m[8] + vector.y * matrix.m[9] + vector.z * matrix.m[10];
            Vector result;
            result.x = x;
            result.y = y;
            result.z = z;
            return result;
        }

        Vector MatrixInverseTransformVector(const Matrix &matrix, const Vector &vector) {
            const float x = vector.x * matrix.m[0] + vector.y * matrix.m[4] + vector.z * matrix.m[8] + (-(matrix.m[0] * matrix.m[3]) - matrix.m[4] * matrix.m[7] - matrix.m[8] * matrix.m[11]);
            const float y = vector.x * matrix.m[1] + vector.y * matrix.m[5] + vector.z * matrix.m[9] + (-(matrix.m[1] * matrix.m[3]) - matrix.m[5] * matrix.m[7] - matrix.m[9] * matrix.m[11]);
            const float z = vector.x * matrix.m[2] + vector.y * matrix.m[6] + vector.z * matrix.m[10] + (-(matrix.m[2] * matrix.m[3]) - matrix.m[6] * matrix.m[7] - matrix.m[10] * matrix.m[11]);
            Vector result;
            result.x = x;
            result.y = y;
            result.z = z;
            return result;
        }

        Vector MatrixInverseRotateVector(const Matrix &matrix, const Vector &vector) {
            const float x = vector.x * matrix.m[0] + vector.y * matrix.m[4] + vector.z * matrix.m[8];
            const float y = vector.x * matrix.m[1] + vector.y * matrix.m[5] + vector.z * matrix.m[9];
            const float z = vector.x * matrix.m[2] + vector.y * matrix.m[6] + vector.z * matrix.m[10];
            Vector result;
            result.x = x;
            result.y = y;
            result.z = z;
            return result;
        }

        Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z) {
            const float sx = ::sinf(x), sy = ::sinf(y), sz = ::sinf(z);
            const float cx = ::cosf(x), cy = ::cosf(y), cz = ::cosf(z);

            matrix.m[0] = cy * cz;
            matrix.m[1] = -(sz * cy);
            matrix.m[2] = sy;
            matrix.m[4] = sy * (sx * cz) + sz * cx;
            matrix.m[5] = cx * cz - (sx * sy) * sz;
            matrix.m[6] = -(sx * cy);
            matrix.m[8] = sx * sz - sy * (cx * cz);
            matrix.m[9] = sx * cz + sz * (sy * cx);
            matrix.m[10] = cx * cy;
            return matrix;
        }

        Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z, RotationOrder order) {
            const float sx = ::sinf(x), sy = ::sinf(y), sz = ::sinf(z);
            const float cx = ::cosf(x), cy = ::cosf(y), cz = ::cosf(z);

            switch (order) {
                case ROTATION_ORDER_XYZ:
                    matrix.m[0] = cy * cz;
                    matrix.m[1] = -(sz * cy);
                    matrix.m[2] = sy;
                    matrix.m[4] = sy * (sx * cz) + sz * cx;
                    matrix.m[5] = cx * cz - (sx * sy) * sz;
                    matrix.m[6] = -(sx * cy);
                    matrix.m[8] = sx * sz - sy * (cx * cz);
                    matrix.m[9] = sx * cz + sz * (sy * cx);
                    matrix.m[10] = cx * cy;
                    return matrix;

                case ROTATION_ORDER_XZY:
                    matrix.m[0] = cy * cz;
                    matrix.m[1] = -sz;
                    matrix.m[2] = sy * cz;
                    matrix.m[4] = sx * sy + sz * (cx * cy);
                    matrix.m[5] = cx * cz;
                    matrix.m[6] = sz * (sy * cx) - sx * cy;
                    matrix.m[8] = sz * (sx * cy) - sy * cx;
                    matrix.m[9] = sx * cz;
                    matrix.m[10] = cx * cy + (sx * sy) * sz;
                    return matrix;

                case ROTATION_ORDER_YXZ:
                    matrix.m[0] = cy * cz + sx * (sy * sz);
                    matrix.m[1] = sy * (sx * cz) - sz * cy;
                    matrix.m[2] = sy * cx;
                    matrix.m[4] = sz * cx;
                    matrix.m[5] = cx * cz;
                    matrix.m[6] = -sx;
                    matrix.m[8] = sz * (sx * cy) - sy * cz;
                    matrix.m[9] = sy * sz + sx * (cy * cz);
                    matrix.m[10] = cx * cy;
                    return matrix;

                case ROTATION_ORDER_YZX:
                    matrix.m[0] = cy * cz;
                    matrix.m[1] = sx * sy - cx * (sz * cy);
                    matrix.m[2] = (sx * sz) * cy + sy * cx;
                    matrix.m[4] = sz;
                    matrix.m[5] = cx * cz;
                    matrix.m[6] = -(sx * cz);
                    matrix.m[8] = -(sy * cz);
                    matrix.m[9] = sx * cy + cx * (sy * sz);
                    matrix.m[10] = cx * cy - (sx * sy) * sz;
                    return matrix;

                case ROTATION_ORDER_ZXY:
                    matrix.m[0] = cy * cz - sx * (sy * sz);
                    matrix.m[1] = -(sz * cx);
                    matrix.m[2] = sy * cz + (sx * sz) * cy;
                    matrix.m[4] = sy * (sx * cz) + sz * cy;
                    matrix.m[5] = cx * cz;
                    matrix.m[6] = sy * sz - sx * (cy * cz);
                    matrix.m[8] = -(sy * cx);
                    matrix.m[9] = sx;
                    matrix.m[10] = cx * cy;
                    return matrix;

                case ROTATION_ORDER_ZYX:
                    matrix.m[0] = cy * cz;
                    matrix.m[1] = sy * (sx * cz) - sz * cx;
                    matrix.m[2] = sy * (cx * cz) + sx * sz;
                    matrix.m[4] = sz * cy;
                    matrix.m[5] = cx * cz + sx * (sy * sz);
                    matrix.m[6] = cx * (sy * sz) - sx * cz;
                    matrix.m[8] = -sy;
                    matrix.m[9] = sx * cy;
                    matrix.m[10] = cx * cy;
                    return matrix;
            }

            return matrix;
        }

        Matrix MatrixSetRotation(Matrix &matrix, const Vector &dir) {
            Vector right = VectorNormalize(VectorCross(Vector{0.0f, 1.0f, 0.0f}, dir));
            Vector up = VectorNormalize(VectorCross(dir, right));
            return MatrixSetRotation(matrix, right, up, dir);
        }

        Matrix MatrixSetRotation(Matrix &matrix, const Vector &right, const Vector &up, const Vector &dir) {
            matrix.m[0] = right.x;
            matrix.m[4] = right.y;
            matrix.m[8] = right.z;
            matrix.m[1] = up.x;
            matrix.m[5] = up.y;
            matrix.m[9] = up.z;
            matrix.m[2] = dir.x;
            matrix.m[6] = dir.y;
            matrix.m[10] = dir.z;
            return matrix;
        }

        Matrix MatrixSetScaling(Matrix &matrix, float x, float y, float z) {
            matrix.m[0] *= x;
            matrix.m[4] *= x;
            matrix.m[8] *= x;
            matrix.m[1] *= y;
            matrix.m[5] *= y;
            matrix.m[9] *= y;
            matrix.m[2] *= z;
            matrix.m[6] *= z;
            matrix.m[10] *= z;
            Vector scale = {x, y, z};
            matrix.scaleVector() = scale;
            return matrix;
        }

        Matrix MatrixSetTranslation(Matrix &matrix, float x, float y, float z) {
            matrix.m[3] = x;
            matrix.m[7] = y;
            matrix.m[11] = z;
            return matrix;
        }

        Matrix MatrixSetTranslation(Matrix &matrix, const Vector &translation) {
            matrix.m[3] = translation.x;
            matrix.m[7] = translation.y;
            matrix.m[11] = translation.z;
            return matrix;
        }

        Matrix MatrixGetInverse(const Matrix &matrix) {
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

            result.m[3] = -(matrix.m[3] * matrix.m[0]) - matrix.m[7] * matrix.m[4] - matrix.m[11] * matrix.m[8];
            result.m[7] = -(matrix.m[3] * matrix.m[1]) - matrix.m[7] * matrix.m[5] - matrix.m[11] * matrix.m[9];
            result.m[11] = -(matrix.m[3] * matrix.m[2]) - matrix.m[7] * matrix.m[6] - matrix.m[11] * matrix.m[10];

            return result;
        }

        Matrix MatrixGetLookAt(const Vector &position, const Vector &target, const Vector &up) {
            Vector dir = VectorNormalize(position - target);
            Vector right = VectorNormalize(VectorCross(up, dir));
            Vector newUp = VectorCross(dir, right);

            return Matrix(right.x, newUp.x, dir.x, position.x,
                          right.y, newUp.y, dir.y, position.y,
                          right.z, newUp.z, dir.z, position.z);
        }

        void MatrixGetGL(const Matrix &matrix, float *out) {
            out[0] = matrix.m[0];
            out[4] = matrix.m[1];
            out[8] = matrix.m[2];
            out[12] = matrix.m[3];
            out[1] = matrix.m[4];
            out[5] = matrix.m[5];
            out[9] = matrix.m[6];
            out[13] = matrix.m[7];
            out[2] = matrix.m[8];
            out[6] = matrix.m[9];
            out[10] = matrix.m[10];
            out[14] = matrix.m[11];
            out[3] = 0.0f;
            out[7] = 0.0f;
            out[11] = 0.0f;
            out[15] = 1.0f;
        }

        void MatrixDebugOut(const Matrix &matrix) {
            (void) matrix;
        }
    }
}
