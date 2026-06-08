#include "class.h"

namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
} }
using AbyssEngine::AEMath::Vector;

struct VecArray;  // Array<Vector>
struct VecPtrArray;  // Array<Vector*>

// Array<Vector>::Array(Array*)                              0x769a8
extern "C" void Gun_VecArray_ctor(void *a);
// Array<Vector*>::Array(Array*)                             0x74530
extern "C" void Gun_VecPtrArray_ctor(void *a);
// ArraySetLength<Vector>(int, Array*)                       0x769b4
extern "C" void Gun_VecArray_setLength(int n, void *a);
// ArraySetLength<Vector*>(int, Array*)                      0x7453c
extern "C" void Gun_VecPtrArray_setLength(int n, void *a);
// Vector::operator=(Vector*, Vector const&)                 0x6eb3c
extern "C" float Vector_assign(Vector *dst, const Vector *src);
// Vector::operator*=(Vector*, float)                        0x72628
extern "C" void Vector_mulAssign(Vector *dst, float s);

extern "C" void *Gun_operator_new(unsigned size);       // operator new(uint)   0x6eb24
extern "C" void *Gun_operator_new_arr(unsigned size);   // operator new[](uint) 0x6ec08

extern "C" Gun *Gun_ctor(Gun *self, int kind, int p2, unsigned count, int p4,
                         int p5, int p6, float p7, Vector dir, Vector vel)
{
    char *s = (char *)self;
    Gun_VecArray_ctor(s + 0x8);
    Gun_VecArray_ctor(s + 0x14);
    Gun_VecArray_ctor(s + 0x20);
    Gun_VecArray_ctor(s + 0x2c);
    F<int>(self, 0x7c) = 0;
    F<int>(self, 0x80) = 0;
    F<int>(self, 0x84) = 0;
    F<int>(self, 0x90) = 0;
    F<int>(self, 0x94) = 0;
    F<int>(self, 0x98) = 0;
    F<int>(self, 0x38) = 0;
    F<int>(self, 0x3c) = 0;
    F<int>(self, 0xb4) = 0;
    F<int>(self, 0xb8) = 0;
    F<int>(self, 0x10c) = 0;
    F<int>(self, 0x110) = 0;
    F<int>(self, 0xf4) = kind;
    F<int>(self, 0x60) = p2;
    F<float>(self, 0x50) = p7;
    F<int>(self, 0xe0) = 0;
    F<int>(self, 0xe4) = 0;
    F<int>(self, 0xe8) = 0;
    F<int>(self, 0xec) = 0;
    F<int>(self, 0xd0) = 0;
    F<int>(self, 0xd4) = 0;
    F<int>(self, 0xd8) = 0;
    F<int>(self, 0xdc) = 0;
    F<int>(self, 0xc0) = 0;
    F<int>(self, 0xc4) = 0;
    F<int>(self, 0xc8) = 0;
    F<int>(self, 0xcc) = 0;
    F<int>(self, 0x74) = 0;
    F<int>(self, 0x78) = 0;
    F<uint8_t>(self, 0xf0) = 0;
    F<uint8_t>(self, 0xa8) = 0;
    float sp = Vector_assign((Vector *)(s + 0x7c), &dir);
    Vector_mulAssign(&vel, sp);
    Vector_assign((Vector *)(s + 0xe4), &vel);
    F<int>(self, 0x44) = p5;
    F<int>(self, 0x48) = p6;
    F<int>(self, 0x6c) = p6;
    F<int>(self, 0xa0) = 0;
    F<uint8_t>(self, 0x88) = 0;
    F<uint8_t>(self, 0xa9) = 0;
    F<int>(self, 0x74) = p4;
    F<int>(self, 0x78) = p4 << 1;
    unsigned long long bytes = (unsigned long long)count * 4;
    unsigned alloc = (unsigned)bytes;
    if ((unsigned)(bytes >> 32) != 0)
        alloc = 0xffffffff;
    F<void *>(self, 0x3c) = Gun_operator_new_arr(alloc);
    F<void *>(self, 0x40) = Gun_operator_new_arr(count | ((int)count >> 31));
    void *arr = Gun_operator_new(0xc);
    Gun_VecPtrArray_ctor(arr);
    F<void *>(self, 0xac) = arr;
    Gun_VecArray_setLength(count, s + 0x8);
    Gun_VecArray_setLength(count, s + 0x14);
    Gun_VecArray_setLength(count, s + 0x20);
    Gun_VecArray_setLength(count, s + 0x2c);
    Gun_VecPtrArray_setLength(count, F<void *>(self, 0xac));
    int off = 0;
    for (int i = 0; i < (int)count; i = i + 1) {
        *(int *)(F<int>(self, 0xc) + off) = 0;
        off = off + 0xc;
        ((int *)F<int>(self, 0x3c))[i] = 0;
        ((uint8_t *)F<int>(self, 0x40))[i] = 0;
        ((int *)*(int *)(F<int>(self, 0xac) + 4))[i] = 0;
    }
    F<int>(self, 0xb8) = 0;
    F<uint8_t>(self, 0x54) = 0;
    F<uint16_t>(self, 0x4c) = 0;
    F<int>(self, 0xb4) = 0;
    F<int>(self, 0x58) = -1;
    F<int>(self, 0x5c) = -1;
    F<uint8_t>(self, 0x68) = 0;
    F<uint16_t>(self, 0xf8) = 1;
    F<int>(self, 0xfc) = 0;
    F<uint8_t>(self, 0x89) = 0;
    F<int>(self, 0x4) = 0;
    F<int>(self, 0xb0) = 0;
    F<int>(self, 0x64) = 0;
    F<int>(self, 0xa4) = 0;
    return self;
}
