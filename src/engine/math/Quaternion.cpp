#include "engine/math/Quaternion.h"
#include <arm_neon.h>
#include <cmath>

namespace AbyssEngine {
    Quaternion::Quaternion() {
    }

    Quaternion::Quaternion(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {
    }

    Quaternion::Quaternion(AEMath::Vector angles) {
        Set(angles);
    }

    void Quaternion::Set(AEMath::Vector angles) {
        Set(angles.x, angles.y, angles.z);
    }

    Quaternion::Quaternion(Quaternion *other) {
        (void) other;
    }

    Quaternion::Quaternion(const AEMath::Matrix &matrix) {
        Set(matrix);
    }

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

    Quaternion::~Quaternion() {
    }

    void Quaternion::Set(float x_angle, float y_angle, float z_angle) {
        float halfZ = z_angle * 0.5f;
        float cosZ = cosf(halfZ);
        float halfY = y_angle * 0.5f;
        float cosY = cosf(halfY);
        float halfX = x_angle * 0.5f;
        float cosX = cosf(halfX);
        float sinZ = sinf(halfZ);
        float sinY = sinf(halfY);
        float cosYsinZ = cosY * sinZ;
        float sinZsinY = sinZ * sinY;
        float cosZsinY = cosZ * sinY;
        float sinX = sinf(halfX);
        float cosZcosY = cosZ * cosY;
        x = cosX * sinZsinY - cosZcosY * sinX;
        y = -(cosYsinZ * sinX) - cosX * cosZsinY;
        z = cosZsinY * sinX - cosX * cosYsinZ;
        w = sinZsinY * sinX + cosZcosY * cosX;
    }

    Quaternion Quaternion::Inverse() const {
        float inv = 1.0f / (x * x + y * y + z * z + w * w);
        Quaternion result;
        result.x = -inv * x;
        result.y = -inv * y;
        result.z = -inv * z;
        result.w = w * inv;
        return result;
    }

    float Quaternion::Dot(const Quaternion &a, const Quaternion &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    Quaternion Quaternion::Lerp(const Quaternion &a, const Quaternion &b, float t) {
        float32x4_t result;
        Quaternion difference = b - a;
        result = vmlaq_f32(vld1q_f32(&a.x), vdupq_n_f32(t), *reinterpret_cast<float32x4_t *>(&difference));
        return reinterpret_cast<Quaternion &>(result).Normalized();
    }

    Quaternion Quaternion::Normalized() {
        float length = Length();
        x /= length;
        y /= length;
        z /= length;
        w /= length;
        return *this;
    }

    Quaternion Quaternion::Lerp(const float *a, const float *b, float t) {
        float32x4_t av = vld1q_f32(a);
        float32x4_t bv = vld1q_f32(b);

        float dot = vgetq_lane_f32(av, 0) * vgetq_lane_f32(bv, 0);
        dot += vgetq_lane_f32(av, 1) * vgetq_lane_f32(bv, 1);
        dot += vgetq_lane_f32(av, 2) * vgetq_lane_f32(bv, 2);
        dot += vgetq_lane_f32(av, 3) * vgetq_lane_f32(bv, 3);

        // Take the shorter way round.
        float32x4_t delta;
        if (dot < 0.0f)
            delta = vsubq_f32(vnegq_f32(bv), av);
        else
            delta = vsubq_f32(bv, av);

        float32x4_t result = vmlaq_f32(av, vdupq_n_f32(t), delta);
        return reinterpret_cast<Quaternion &>(result).Normalized();
    }

    Quaternion::operator float *() { return &x; }

    Quaternion::operator const float *() const { return &x; }

    float &Quaternion::operator[](int i) { return (&x)[i]; }

    float Quaternion::operator[](int i) const { return (&x)[i]; }

    float Quaternion::Length() const {
        return sqrtf(x * x + y * y + z * z + w * w);
    }

    void Quaternion::Convert(AEMath::Matrix &matrix) {
        float qw = w;
        float qx = x;
        float qy = y;
        float qz = z;
        double sqx = qx * qx;
        double sqy = qy * qy;
        double sqz = qz * qz;
        double sqw = qw * qw;
        double invs = 1.0 / (sqx + sqy + sqz + sqw);
        matrix.m[0] = (sqx - sqy - sqz + sqw) * invs;
        matrix.m[5] = (-sqx + sqy - sqz + sqw) * invs;
        matrix.m[10] = (-sqx - sqy + sqz + sqw) * invs;

        double tmp1 = qx * qy;
        double tmp2 = qw * qz;
        matrix.m[1] = 2.0 * (tmp1 + tmp2) * invs;
        matrix.m[4] = 2.0 * (tmp1 - tmp2) * invs;

        tmp1 = qx * qz;
        tmp2 = qw * qy;
        matrix.m[2] = 2.0 * (tmp1 - tmp2) * invs;
        matrix.m[8] = 2.0 * (tmp1 + tmp2) * invs;

        tmp1 = qy * qz;
        tmp2 = qw * qx;
        matrix.m[6] = 2.0 * (tmp1 + tmp2) * invs;
        matrix.m[9] = 2.0 * (tmp1 - tmp2) * invs;
    }
}
