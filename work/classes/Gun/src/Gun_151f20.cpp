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
    self->f_7c = 0;
    self->f_80 = 0;
    self->f_84 = 0;
    self->f_90 = 0;
    self->f_94 = 0;
    self->f_98 = 0;
    self->f_38 = 0;
    self->f_3c = 0;
    self->f_b4 = 0;
    self->f_b8 = 0;
    self->f_10c = 0;
    self->f_110 = 0;
    self->f_f4 = kind;
    self->f_60 = p2;
    self->f_50 = p7;
    self->f_e0 = 0;
    self->f_e4 = 0;
    self->f_e8 = 0;
    self->f_ec = 0;
    self->f_d0 = 0;
    self->f_d4 = 0;
    self->f_d8 = 0;
    self->f_dc = 0;
    self->f_c0 = 0;
    self->f_c4 = 0;
    self->f_c8 = 0;
    self->f_cc = 0;
    self->f_74 = 0;
    self->f_78 = 0;
    self->f_f0 = 0;
    self->f_a8 = 0;
    float sp = Vector_assign((Vector *)(s + 0x7c), &dir);
    Vector_mulAssign(&vel, sp);
    Vector_assign((Vector *)(s + 0xe4), &vel);
    self->f_44 = p5;
    self->f_48 = p6;
    self->f_6c = p6;
    self->f_a0 = 0;
    self->f_88 = 0;
    F<uint8_t>(self, 0xa9) = 0;
    self->f_74 = p4;
    self->f_78 = p4 << 1;
    unsigned long long bytes = (unsigned long long)count * 4;
    unsigned alloc = (unsigned)bytes;
    if ((unsigned)(bytes >> 32) != 0)
        alloc = 0xffffffff;
    self->f_3c = Gun_operator_new_arr(alloc);
    self->f_40 = Gun_operator_new_arr(count | ((int)count >> 31));
    void *arr = Gun_operator_new(0xc);
    Gun_VecPtrArray_ctor(arr);
    self->f_ac = arr;
    Gun_VecArray_setLength(count, s + 0x8);
    Gun_VecArray_setLength(count, s + 0x14);
    Gun_VecArray_setLength(count, s + 0x20);
    Gun_VecArray_setLength(count, s + 0x2c);
    Gun_VecPtrArray_setLength(count, self->f_ac);
    int off = 0;
    for (int i = 0; i < (int)count; i = i + 1) {
        *(int *)(self->f_c + off) = 0;
        off = off + 0xc;
        ((int *)self->f_3c)[i] = 0;
        ((uint8_t *)self->f_40)[i] = 0;
        ((int *)*(int *)(self->f_ac + 4))[i] = 0;
    }
    self->f_b8 = 0;
    self->f_54 = 0;
    self->f_4c = 0;
    self->f_b4 = 0;
    self->f_58 = -1;
    self->f_5c = -1;
    self->f_68 = 0;
    self->f_f8 = 1;
    self->f_fc = 0;
    F<uint8_t>(self, 0x89) = 0;
    self->f_4 = 0;
    self->f_b0 = 0;
    self->f_64 = 0;
    self->f_a4 = 0;
    return self;
}
