#ifndef GOF2_QUATERNION_H
#define GOF2_QUATERNION_H
#include "gof2/common.h"
// real struct kept from byte-match recovery
struct Quaternion {
    float x;
    float y;
    float z;
    float w;

    Quaternion();
    Quaternion(float x_, float y_, float z_, float w_);
    Quaternion(AEMath::Vector angles);
    Quaternion(Quaternion *other);
    Quaternion(const AEMath::Matrix &matrix);
    ~Quaternion();

    void Set(AEMath::Vector angles);
    void Set(const AEMath::Matrix &matrix);
    void Set(float x_angle, float y_angle, float z_angle);

    Quaternion Inverse() const;
    static float Dot(const Quaternion &a, const Quaternion &b);
    void Lerp(const Quaternion &a, const Quaternion &b, float t);
    Quaternion Normalized();
    void Lerp(const float *a, const float *b, float t);

    operator float *();
    operator const float *() const;
    float &operator[](int index);
    float operator[](int index) const;

    float Length() const;
    void Convert(AEMath::Matrix &matrix);
};
#endif
