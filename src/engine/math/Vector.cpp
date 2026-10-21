#include "engine/math/Vector.h"
#include "engine/math/AEMath.h"

#include <cmath>

// Original Vector.cpp TU (Android .text 0x7395d-0x73f31): accessors, operators and Vector* helpers.
namespace AbyssEngine {
    namespace AEMath {
        Vector::operator float *() { return &x; }
        Vector::operator const float *() const { return &x; }

        float &Vector::operator[](int i) { return (&x)[i]; }
        float Vector::operator[](int i) const { return (&x)[i]; }

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
    }
}
