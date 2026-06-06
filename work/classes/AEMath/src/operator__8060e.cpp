#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector operator*(const Matrix &matrix, const Vector &vector)
{
    Vector result;

    const volatile float *v = &vector.y;
    const volatile float *mx = &matrix.m[1];
    const volatile float *my = &matrix.m[5];
    const volatile float *mz = &matrix.m[9];

    float vy = v[0];
    float m1 = mx[0];
    float m5 = my[0];
    float m9 = mz[0];

    float x = vy * m1;
    float y = vy * m5;
    float z = vy * m9;

    float vx = v[-1];
    float m0 = mx[-1];
    float m4 = my[-1];
    float m8 = mz[-1];

    x += vx * m0;
    y += vx * m4;
    z += vx * m8;

    float vz = v[1];
    float m2 = mx[1];
    float m6 = my[1];
    float m10 = mz[1];
    float m3 = mx[2];
    float m7 = my[2];
    float m11 = mz[2];

    x += vz * m2;
    y += vz * m6;
    z += vz * m10;

    result.x = m3 + x;
    result.y = m7 + y;
    result.z = m11 + z;

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
