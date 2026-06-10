// gof2/math.h — AbyssEngine math types (single definition for the native build).
// Vector/Matrix live in AbyssEngine::AEMath; Quaternion in AbyssEngine. Method bodies are provided
// by the merged native/src/AEMath.cpp + Quaternion.cpp. Natural 64-bit layout (no byte-matching).
#ifndef GOF2_MATH_H
#define GOF2_MATH_H
#include <cstdint>

namespace AbyssEngine {
namespace AEMath {

struct Vector {
    float x, y, z;
    operator float*()             { return &x; }
    operator const float*() const { return &x; }
    float&       operator[](int i)       { return (&x)[i]; }
    float        operator[](int i) const { return (&x)[i]; }
    Vector& operator=(const Vector& o);
    Vector& operator+=(const Vector& o);
    Vector& operator-=(const Vector& o);
    Vector& operator*=(float s);
    Vector& operator*=(const Vector& o);
    Vector& operator/=(float s);
    Vector& operator/=(const Vector& o);
};

struct Matrix {
    float m[16];
    Matrix() {}
    operator float*()             { return m; }
    operator const float*() const { return m; }
    float&       operator[](int i)       { return m[i]; }
    float        operator[](int i) const { return m[i]; }
    Matrix& operator=(const Matrix& o);
    Matrix& operator*=(const Matrix& o);

    // ---- methods (converted from String_*/free functions) ----
    void initIdentity();
};

} // namespace AEMath

struct Quaternion {
    float x, y, z, w;
    Quaternion() {}
    Quaternion(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    Quaternion(AEMath::Vector angles);
    Quaternion(Quaternion* other);
    Quaternion(const AEMath::Matrix& matrix);
    ~Quaternion() {}
    void Set(AEMath::Vector angles);
    void Set(const AEMath::Matrix& matrix);
    void Set(float xa, float ya, float za);
    Quaternion Inverse() const;
    static float Dot(const Quaternion& a, const Quaternion& b);
    void Lerp(const Quaternion& a, const Quaternion& b, float t);
    void Lerp(const float* a, const float* b, float t);
    Quaternion Normalized();
    float Length() const;
    void Convert(AEMath::Matrix& matrix);
    operator float*()             { return &x; }
    operator const float*() const { return &x; }
    float&       operator[](int i)       { return (&x)[i]; }
    float        operator[](int i) const { return (&x)[i]; }
};

} // namespace AbyssEngine
#endif // GOF2_MATH_H
