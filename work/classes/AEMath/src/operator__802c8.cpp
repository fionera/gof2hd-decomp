#include "class.h"

namespace AbyssEngine {
namespace AEMath {

extern "C" unsigned int __stack_chk_guard;
extern "C" void __stack_chk_fail() __attribute__((noreturn));

inline Matrix::Matrix() {}

Matrix operator*(const Matrix &lhs, const Matrix &rhs)
{
    Vector scale;
    volatile unsigned int guard = __stack_chk_guard;
    unsigned int one = 0x3f800000;
    Matrix result;

    float a9 = lhs.m[9];
    float b6 = rhs.m[6];
    float a1 = lhs.m[1];
    float a5 = lhs.m[5];
    float r10 = b6 * a9;
    float r2 = a1 * b6;
    float b5 = rhs.m[5];
    float r6 = b6 * a5;
    float b7 = rhs.m[7];
    float b4 = rhs.m[4];
    float r1 = a1 * b5;
    float b2 = rhs.m[2];
    float r11 = b7 * a9;
    float a8 = lhs.m[8];
    float r4 = b4 * a5;
    float a4 = lhs.m[4];
    float r9 = b5 * a9;
    float a0 = lhs.m[0];
    r10 += b2 * a8;
    r6 += b2 * a4;
    float b3 = rhs.m[3];
    r2 += a0 * b2;
    float a10 = lhs.m[10];
    float r5 = b5 * a5;
    float a6 = lhs.m[6];
    float r7 = b7 * a5;
    float b11 = rhs.m[11];
    float r3 = a1 * b7;
    float a2 = lhs.m[2];
    r11 += b3 * a8;
    float b1 = rhs.m[1];
    float r8 = b4 * a9;
    float b0 = rhs.m[0];
    float r0 = a1 * b4;
    float b10 = rhs.m[10];
    float r4v = b4 * a4;
    float b8 = rhs.m[8];
    r7 += b3 * a4;
    float b9 = rhs.m[9];
    r3 += a0 * b3;
    float a3 = lhs.m[3];
    r11 += b11 * a10;
    r1 += a0 * b1;
    r9 += b1 * a8;
    r5 += b1 * a4;
    float a7 = lhs.m[7];
    r7 += b11 * a6;
    r3 += a2 * b11;
    r8 += b0 * a8;
    r0 += a0 * b0;
    float a11 = lhs.m[11];
    r10 += b10 * a10;
    r4v += b0 * a4;
    *reinterpret_cast<unsigned int *>(&result.m[13]) = one;
    *reinterpret_cast<unsigned int *>(&result.m[14]) = one;
    r2 += a2 * b10;
    r1 += a2 * b9;
    r5 += b9 * a6;
    r0 += a2 * b8;
    r6 += b10 * a6;
    r8 += b8 * a10;
    r9 += b9 * a10;

    result.m[0] = r0;
    result.m[1] = r1;
    result.m[2] = r2;
    result.m[3] = a3 + r3;
    result.m[4] = r4v;
    result.m[5] = r5;
    result.m[6] = r6;
    result.m[7] = a7 + r7;
    result.m[8] = r8;
    result.m[9] = r9;
    result.m[10] = r10;
    result.m[11] = a11 + r11;

    *reinterpret_cast<unsigned int *>(&result.m[12]) = one;
    scale = reinterpret_cast<const Vector &>(lhs.m[12]) * reinterpret_cast<const Vector &>(rhs.m[12]);
    reinterpret_cast<Vector &>(result.m[12]) = scale;

    if (__stack_chk_guard != guard) {
        __stack_chk_fail();
    }

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
