// mathtypes.h — AbyssEngine math types (single definition for the native build).
// Vector/Matrix live in AbyssEngine::AEMath; Quaternion in AbyssEngine. Method bodies are provided
// by the merged native/src/AEMath.cpp + Quaternion.cpp. Natural 64-bit layout (no byte-matching).
#ifndef GOF2_MATH_H
#define GOF2_MATH_H
#include <cstdint>
#include <cmath>     // inline math (sqrtf/sinf/...) and the engine's math users that reached it via common.h

namespace AbyssEngine {
namespace AEMath {

struct Vector {
    float x, y, z;
    operator float*();                   // out-of-line (the binary emits these accessors)
    operator const float*() const;
    float&       operator[](int i);
    float        operator[](int i) const;
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
    Matrix();                            // out-of-line: initializes to the identity transform
    operator float*();                   // out-of-line (the binary emits these accessors)
    operator const float*() const;
    float&       operator[](int i)       { return m[i]; }
    float        operator[](int i) const { return m[i]; }
    Matrix& operator=(const Matrix& o);
    Matrix& operator*=(const Matrix& o);
};

// Engine math free-functions (defined in libgof2hd; calls re-mangle to AbyssEngine::AEMath::*)
float Sinf(float);
float Cosf(float);
float ATanf(float);
float Pow(float, float);
} // namespace AEMath

struct Quaternion {
    float x, y, z, w;
    Quaternion();                                        // out-of-line so the binary's C1/C2 symbols emit
    Quaternion(float x_, float y_, float z_, float w_);  // out-of-line (matches the original)
    Quaternion(AEMath::Vector angles);
    Quaternion(Quaternion* other);
    Quaternion(const AEMath::Matrix& matrix);
    ~Quaternion();                                       // out-of-line (the binary emits D1/D2)
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
    operator float*();                   // out-of-line (the binary emits these accessors)
    operator const float*() const;
    float&       operator[](int i);
    float        operator[](int i) const;
};

} // namespace AbyssEngine
#endif // GOF2_MATH_H
