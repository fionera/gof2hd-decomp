#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Matrix MatrixGetInverse(const Matrix &matrix)
{
    Matrix result;

    result.m[0] = matrix.m[0];
    result.m[1] = matrix.m[4];
    result.m[2] = matrix.m[8];

    result.m[4] = matrix.m[1];
    result.m[5] = matrix.m[5];
    result.m[6] = matrix.m[9];

    result.m[8] = matrix.m[2];
    result.m[9] = matrix.m[6];
    result.m[10] = matrix.m[10];

    float x = -(matrix.m[3] * matrix.m[0]) - (matrix.m[7] * matrix.m[4]) - (matrix.m[11] * matrix.m[8]);
    float y = -(matrix.m[3] * matrix.m[1]) - (matrix.m[7] * matrix.m[5]) - (matrix.m[11] * matrix.m[9]);
    float z = -(matrix.m[3] * matrix.m[2]) - (matrix.m[7] * matrix.m[6]) - (matrix.m[11] * matrix.m[10]);

    result.m[3] = x;
    result.m[7] = y;
    result.m[11] = z;

    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
