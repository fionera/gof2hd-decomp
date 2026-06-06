#ifndef GOF2_DECOMP_QUATERNION_H
#define GOF2_DECOMP_QUATERNION_H

extern "C" float sqrtf(float);
extern "C" float sinf(float);
extern "C" float cosf(float);

namespace AbyssEngine {

namespace AEMath {

struct Vector {
    float x;
    float y;
    float z;
};

struct Matrix {
    float m[16];
};

} // namespace AEMath

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

static_assert(sizeof(AEMath::Vector) == 0xc, "AEMath::Vector layout");
static_assert(sizeof(AEMath::Matrix) == 0x40, "AEMath::Matrix layout");
static_assert(sizeof(Quaternion) == 0x10, "Quaternion layout");
static_assert(__builtin_offsetof(Quaternion, x) == 0x0, "Quaternion::x offset");
static_assert(__builtin_offsetof(Quaternion, y) == 0x4, "Quaternion::y offset");
static_assert(__builtin_offsetof(Quaternion, z) == 0x8, "Quaternion::z offset");
static_assert(__builtin_offsetof(Quaternion, w) == 0xc, "Quaternion::w offset");

} // namespace AbyssEngine

#endif
