#ifndef GOF2_TRANSFORM_H
#define GOF2_TRANSFORM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
using uint = uint32_t;
using longlong = int64_t;
using ulonglong = uint64_t;

void *operator new(__SIZE_TYPE__ size);
inline void *operator new(__SIZE_TYPE__, void *ptr) { return ptr; }
void operator delete(void *ptr) noexcept;



template <class T> void ArrayAdd(T value, Array<T> &array);
template <class T> void ArraySet(Array<T> *src, Array<T> *dst);
template <class T> void ArrayReleaseClasses(Array<T> *array);

namespace AbyssEngine {

struct Camera;
struct KeyFrame;
struct Mesh;
struct Transform;

namespace AEMath {





struct BSphere {
    Vector center;
    float radius;
    float radius2;

    BSphere &operator=(const BSphere &other);
    void Merge(const BSphere &other);
};

Vector VectorLerp(const Vector &from, const Vector &to, float t);
Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector);
Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector);
Matrix MatrixSetScaling(Matrix &matrix, float x, float y, float z);
Matrix MatrixSetTranslation(Matrix &matrix, float x, float y, float z);
Matrix operator*(const Matrix &lhs, const Matrix &rhs);

} // namespace AEMath



struct KeyFrame {
    KeyFrame();
};

struct Mesh {
    Mesh(Mesh *other);
};

enum AnimationMode {
    AnimationMode_0 = 0
};



int CameraIsSphereinViewFrustum(const AEMath::Vector &center, float radius, Camera *camera);

} // namespace AbyssEngine

extern "C" float __aeabi_l2f(longlong value);
extern "C" void __stack_chk_fail();
extern "C" int *g_transform_insert_counter;
extern "C" bool g_transform_matrix_flag;

struct Transform {
    uint8_t field_0x0;                  // +0x0
    uint16_t field_0x2;                 // +0x2
    uint32_t field_0x4;                 // +0x4
    uint32_t field_0x8;                 // +0x8
    uint32_t field_0xc;                 // +0xc
    uint32_t field_0x10;                // +0x10
    uint32_t field_0x14;                // +0x14
    uint32_t field_0x18;                // +0x18
    uint16_t field_0x28;                // +0x28
    uint16_t field_0x2a;                // +0x2a
    uint32_t field_0x2c;                // +0x2c
    uint32_t field_0x30;                // +0x30
    Transform* field_0x34;              // +0x34
    uint field_0x3c;                    // +0x3c
    char** field_0x40;                  // +0x40
    uint field_0x4c;                    // +0x4c
    Transform** field_0x50;             // +0x50
    uint8_t field_0x5c;                 // +0x5c
    uint32_t field_0x60;                // +0x60
    uint32_t field_0x64;                // +0x64
    uint32_t field_0x68;                // +0x68
    uint32_t field_0x6c;                // +0x6c
    uint32_t field_0x70;                // +0x70
    uint32_t field_0x74;                // +0x74
    uint32_t field_0x78;                // +0x78
    uint32_t field_0x80;                // +0x80
    unsigned char field_0x84;           // +0x84
    uint8_t field_0x85;                 // +0x85
    AEMath::Vector field_0xd4;          // +0xd4
    float field_0xe0;                   // +0xe0
    float field_0xe4;                   // +0xe4
    int field_0xe8;                     // +0xe8
    bool field_0xec;                    // +0xec
    bool field_0xed;                    // +0xed
    float field_0xf0;                   // +0xf0
    longlong field_0xf8;                // +0xf8
    longlong field_0x100;               // +0x100
    longlong field_0x108;               // +0x108
    longlong field_0x110;               // +0x110
    int field_0x11c;                    // +0x11c
    int field_0x120;                    // +0x120
    bool field_0x17c;                   // +0x17c
};
#endif
