#include "class.h"

namespace AbyssEngine {
namespace AEMath {

void MatrixGetGL(const Matrix &matrix, float *out)
{
    volatile float *dst = out;

    dst[0] = matrix.m[0];
    dst[4] = matrix.m[1];
    dst[8] = matrix.m[2];
    dst[12] = matrix.m[3];
    dst[1] = matrix.m[4];
    dst[5] = matrix.m[5];
    dst[9] = matrix.m[6];
    dst[13] = matrix.m[7];
    dst[2] = matrix.m[8];
    dst[6] = matrix.m[9];
    dst[10] = matrix.m[10];
    float z = matrix.m[11];
    dst[3] = 0.0f;
    dst[14] = z;
    dst[7] = 0.0f;
    dst[11] = 0.0f;
    dst[15] = 1.0f;
}

} // namespace AEMath
} // namespace AbyssEngine
