#include "engine/math/Vector.h"
#include "engine/math/AEMath.h"
#include "engine/math/BSphere.h"
#include "engine/math/Matrix.h"
#include "engine/math/Transform.h"

#include <cmath>

// Original Vector.cpp TU (Android .text 0x7395d-0x73f31): accessors, operators and Vector* helpers.
namespace AbyssEngine {
    namespace AEMath {
        Vector::operator float *() { return &x; }
        Vector::operator const float *() const { return &x; }

        float &Vector::operator[](int i) { return (&x)[i]; }
        const float &Vector::operator[](int i) const { return (&x)[i]; }

        Vector &Vector::operator=(const Vector &other) {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        Vector &Vector::operator+=(const Vector &other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector &Vector::operator-=(const Vector &other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vector &Vector::operator*=(float scale) {
            x *= scale;
            y *= scale;
            z *= scale;
            return *this;
        }

        Vector &Vector::operator*=(const Vector &other) {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        Vector &Vector::operator/=(float scale) {
            x /= scale;
            y /= scale;
            z /= scale;
            return *this;
        }

        Vector &Vector::operator/=(const Vector &other) {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        Vector operator+(const Vector &value) {
            return value;
        }

        Vector operator-(const Vector &value) {
            Vector result;
            result.x = -value.x;
            result.y = -value.y;
            result.z = -value.z;
            return result;
        }

        Vector operator+(const Vector &lhs, const Vector &rhs) {
            Vector result;
            result.x = lhs.x + rhs.x;
            result.y = lhs.y + rhs.y;
            result.z = lhs.z + rhs.z;
            return result;
        }

        Vector operator-(const Vector &lhs, const Vector &rhs) {
            Vector result;
            result.x = lhs.x - rhs.x;
            result.y = lhs.y - rhs.y;
            result.z = lhs.z - rhs.z;
            return result;
        }

        Vector operator*(const Vector &lhs, const Vector &rhs) {
            Vector result;
            result.x = lhs.x * rhs.x;
            result.y = lhs.y * rhs.y;
            result.z = lhs.z * rhs.z;
            return result;
        }

        Vector operator*(const Vector &lhs, float rhs) {
            Vector result;
            result.x = lhs.x;
            result.y = lhs.y;
            result.z = lhs.z;
            result.x *= rhs;
            result.y *= rhs;
            result.z *= rhs;
            return result;
        }

        Vector operator*(float lhs, const Vector &rhs) {
            Vector result;
            result.x = rhs.x;
            result.y = rhs.y;
            result.z = rhs.z;
            result.x *= lhs;
            result.y *= lhs;
            result.z *= lhs;
            return result;
        }

        Vector operator/(const Vector &lhs, const Vector &rhs) {
            Vector result;
            result.x = lhs.x / rhs.x;
            result.y = lhs.y / rhs.y;
            result.z = lhs.z / rhs.z;
            return result;
        }

        Vector operator/(const Vector &lhs, float rhs) {
            Vector result;
            result.x = lhs.x;
            result.y = lhs.y;
            result.z = lhs.z;
            result.x /= rhs;
            result.y /= rhs;
            result.z /= rhs;
            return result;
        }

        Vector operator/(float lhs, const Vector &rhs) {
            Vector result;
            result.x = rhs.x;
            result.y = rhs.y;
            result.z = rhs.z;
            result.x /= lhs;
            result.y /= lhs;
            result.z /= lhs;
            return result;
        }

        bool operator==(const Vector &lhs, const Vector &rhs) {
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
        }

        bool operator!=(const Vector &lhs, const Vector &rhs) {
            return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
        }

        bool operator<(const Vector &lhs, const Vector &rhs) {
            return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
        }

        bool operator>(const Vector &lhs, const Vector &rhs) {
            return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z;
        }

        bool operator<=(const Vector &lhs, const Vector &rhs) {
            return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z;
        }

        bool operator>=(const Vector &lhs, const Vector &rhs) {
            return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z;
        }

        bool VectorIsEqual(const Vector &lhs, const Vector &rhs) {
            return lhs == rhs;
        }

        float VectorDot(const Vector &lhs, const Vector &rhs) {
            return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
        }

        Vector VectorCross(const Vector &lhs, const Vector &rhs) {
            float x = lhs.y * rhs.z - lhs.z * rhs.y;
            float y = lhs.z * rhs.x - lhs.x * rhs.z;
            float z = lhs.x * rhs.y - lhs.y * rhs.x;
            Vector result;
            result.x = x;
            result.y = y;
            result.z = z;
            return result;
        }

        Vector VectorNormalize(const Vector &value) {
            const float length = ::sqrtf(value.x * value.x + value.y * value.y + value.z * value.z);
            if (length == 0.0f)
                return Vector(0.0f, 1.0f, 0.0f);
            return Vector(value.x / length, value.y / length, value.z / length);
        }

        float VectorLength(const Vector &value) {
            return ::sqrtf(value.x * value.x + value.y * value.y + value.z * value.z);
        }

        Vector VectorLerp(const Vector &from, const Vector &to, float t) {
            return from + (to - from) * t;
        }
        Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector);

        Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector);

        // Vector::operator= is defined in this TU; keep the rotated-radius copy as a three-register
        // store (the original inlines the same field copy).
        static inline Vector &CopyVector(Vector &dst, const Vector &src) {
            dst.x = src.x;
            dst.y = src.y;
            dst.z = src.z;
            return dst;
        }

        void BSphere::Merge(const Transform &t) {
            // World-space bounds of the transform: moved centre, radius scaled by the largest axis.
            BSphere sphere;
            sphere.radius2 = 1.0f;
            {
                Vector center = MatrixTransformVector(t.worldMatrix, t.boundingSphere.center);
                sphere.center.x = center.x;
                sphere.center.y = center.y;
                sphere.center.z = center.z;
            }
            Vector v;
            v.x = t.boundingSphere.radius;
            v.y = t.boundingSphere.radius;
            v.z = t.boundingSphere.radius;
            Vector rotated = MatrixRotateVector(t.worldMatrix, v);
            CopyVector(v, rotated);
            float ax = v.x > 0.0f ? v.x : -v.x;
            float ay = v.y > 0.0f ? v.y : -v.y;
            float az = v.z > 0.0f ? v.z : -v.z;
            if (ax > ay)
                ay = ax;
            if (ay > az)
                az = ay;
            sphere.radius = az;
            Merge(sphere);
        }


void BSphere::Merge(const BSphere &other) {
            if (other.radius2 > this->radius2)
                this->radius2 = other.radius2;

            if (other.radius == 0.0f)
                return;

            if (this->radius == 0.0f) {
                // Empty sphere: take the other one over (the binary copies the fields and then
                // assigns the whole sphere once more).
                this->radius = other.radius;
                this->center.x = other.center.x;
                this->center.y = other.center.y;
                this->center.z = other.center.z;
                this->radius2 = other.radius2;
                *this = other;
                return;
            }

            float dx = other.center.x - this->center.x;
            float dy = other.center.y - this->center.y;
            float dz = other.center.z - this->center.z;
            float dist = std::sqrt(dx * dx + dy * dy + dz * dz);

            float r1 = this->radius;
            float r2 = other.radius;

            if (dist == 0.0f) {
                if (r1 > r2)
                    r2 = r1;
                this->radius = r2;
                return;
            }

            // Already enclosed?
            if (!(r1 <= dist + r2))
                return;

            if (dist - r2 < -r1) {
                this->center.x = other.center.x;
                this->center.y = other.center.y;
                this->center.z = other.center.z;
                this->radius = r2;
                return;
            }

            float t = (((dist + r2) - r1) * 0.5f) / dist;
            this->center.x = this->center.x + dx * t;
            this->center.y = this->center.y + dy * t;
            this->center.z = this->center.z + dz * t;
            this->radius = (dist + r1 + r2) * 0.5f;
        }

        BSphere &BSphere::operator=(const BSphere &other) {
            this->center.x = other.center.x;
            this->center.y = other.center.y;
            this->center.z = other.center.z;
            this->radius = other.radius;
            this->radius2 = other.radius2;
            return *this;
        }
    }
}
