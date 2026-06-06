#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector MatrixInverseRotateVector(const Matrix &matrix, const Vector &vector)
{
    Vector result;
    const volatile float *v = &vector.x;

    float y = v[1];

    float resultX = y * matrix.m[4];
    float resultY = y * matrix.m[5];
    float resultZ = y * matrix.m[6];

    float x = v[0];

    resultX = x * matrix.m[0] + resultX;
    resultY = x * matrix.m[1] + resultY;
    resultZ = x * matrix.m[2] + resultZ;

    float z = v[2];

    resultX = z * matrix.m[8] + resultX;
    resultY = z * matrix.m[9] + resultY;
    resultZ = z * matrix.m[10] + resultZ;

    result.x = resultX;
    result.y = resultY;
    result.z = resultZ;

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
