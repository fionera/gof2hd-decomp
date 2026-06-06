#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector)
{
    const float *m = matrix.m;

    float vy = vector.y;
    float m1 = m[1];
    float m5 = m[5];
    float m9 = m[9];
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    float x = vy * m1;
    float y = vy * m5;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    float vx = vector.x;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    float z = vy * m9;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    float m0 = m[0];
    float m4 = m[4];
    float m8 = m[8];
    float vz = vector.z;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    x += vx * m0;
    float m2 = m[2];
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    y += vx * m4;
    float m6 = m[6];
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    z += vx * m8;
    float m10 = m[10];
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    float tx = m[3];
    float ty = m[7];
    float tz = m[11];
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    x += vz * m2;
    y += vz * m6;
    z += vz * m10;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);

    Vector result;
    result.x = tx + x;
    result.y = ty + y;
    result.z = tz + z;
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
