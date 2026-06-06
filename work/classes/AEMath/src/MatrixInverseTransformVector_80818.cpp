#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector MatrixInverseTransformVector(const Matrix &matrix, const Vector &vector)
{
    Vector result;

    float xBase = -(matrix.m[4] * matrix.m[7]) - matrix.m[0] * matrix.m[3];
    float yBase = -(matrix.m[7] * matrix.m[5]) - matrix.m[3] * matrix.m[1];
    float zBase = -(matrix.m[7] * matrix.m[6]) - matrix.m[3] * matrix.m[2];

    xBase -= matrix.m[8] * matrix.m[11];
    yBase -= matrix.m[11] * matrix.m[9];
    zBase -= matrix.m[11] * matrix.m[10];

    result.x = vector.y * matrix.m[4] + vector.x * matrix.m[0] + vector.z * matrix.m[8] + xBase;
    result.y = vector.y * matrix.m[5] + vector.x * matrix.m[1] + vector.z * matrix.m[9] + yBase;
    result.z = vector.y * matrix.m[6] + vector.x * matrix.m[2] + vector.z * matrix.m[10] + zBase;

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
