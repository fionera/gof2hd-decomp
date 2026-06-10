#ifndef GOF2_PAINTCANVAS_H
#define GOF2_PAINTCANVAS_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
typedef unsigned int size_t;

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }

extern "C" void *memcpy(void *, const void *, size_t);
extern "C" void *memset(void *, int, size_t);
extern "C" int strcmp(const char *, const char *);
extern "C" int strlen(const char *);

namespace AbyssEngine {

struct Engine;


struct Mesh;
struct Material;
struct Resource;
struct ResourceTexture;
struct ResourceMesh;
struct ImageFont;
struct Image2D;
struct Transform;
struct Camera;
struct SpriteSystem;





template <class T>
void ArrayAdd(T item, Array<T> &array);

template <class T>
void ArrayRemoveAt(uint32_t index, Array<T> &array);

namespace AEMath {





struct BSphere {
    Vector center;
    float radius;
    float radius2;
};

Matrix MatrixIdentity(Matrix &matrix);
Matrix MatrixSetTranslation(Matrix &matrix, float x, float y, float z);
Matrix MatrixSetScaling(Matrix &matrix, float x, float y, float z);
Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z);
Matrix MatrixGetInverse(const Matrix &matrix);
void MatrixMultiply(Matrix &lhs, const Matrix &rhs);
void MatrixGetGL(const Matrix &matrix, float *out);
Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector);
Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector);
float VectorDot(const Vector &lhs, const Vector &rhs);
Vector VectorCross(const Vector &lhs, const Vector &rhs);
float VectorLength(const Vector &value);
float Sqrtf(float value);
float Sinf(float value);
float Cosf(float value);
float ACosf(float value);
float ATanf(float value);
float Absf(float value);

} // namespace AEMath

} // namespace AbyssEngine

struct PaintCanvas {
    int field_0x4;                      // +0x4
    char* field_0x8;                    // +0x8
    int field_0xc;                      // +0xc
    unsigned int field_0x10;            // +0x10
    char** field_0x14;                  // +0x14
    int field_0x20;                     // +0x20
    unsigned int field_0x24;            // +0x24
    char** field_0x28;                  // +0x28
    int field_0x30;                     // +0x30
    void* field_0x34;                   // +0x34
    unsigned int field_0x134;           // +0x134
    char** field_0x138;                 // +0x138
    unsigned int field_0x140;           // +0x140
    void** field_0x144;                 // +0x144
    unsigned int field_0x14c;           // +0x14c
    char** field_0x150;                 // +0x150
    unsigned int field_0x158;           // +0x158
    char** field_0x15c;                 // +0x15c
    unsigned int field_0x164;           // +0x164
    void** field_0x168;                 // +0x168
    unsigned int field_0x170;           // +0x170
    unsigned int field_0x174;           // +0x174
    void** field_0x178;                 // +0x178
    unsigned int field_0x180;           // +0x180
    char** field_0x184;                 // +0x184
    char* field_0x1c8;                  // +0x1c8
    unsigned char field_0x1f0;          // +0x1f0
    char field_0x1f1;                   // +0x1f1
    int field_0x1f4;                    // +0x1f4
    float field_0x1fc;                  // +0x1fc
    float field_0x200;                  // +0x200
    float field_0x204;                  // +0x204
    float field_0x208;                  // +0x208
};
#endif
