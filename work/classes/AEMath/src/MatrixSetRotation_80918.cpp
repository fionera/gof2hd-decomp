#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z)
{
    float (*sin_ptr)(float) = ::sinf;
    float sx = sin_ptr(x);
    float (*cos_ptr)(float) = ::cosf;
    float sy = sin_ptr(y);
    float sz = sin_ptr(z);
    float cx = cos_ptr(x);
    float cy = cos_ptr(y);
    float cz = cos_ptr(z);

    matrix.m[0] = cy * cz;
    matrix.m[1] = -(sz * cy);
    matrix.m[2] = sy;
    matrix.m[8] = sx * sz - sy * (cx * cz);
    matrix.m[9] = sx * cz + sz * (sy * cx);
    matrix.m[10] = cx * cy;
    matrix.m[4] = sy * (sx * cz) + sz * cx;
    matrix.m[5] = cx * cz - (sx * sy) * sz;
    matrix.m[6] = -(sx * cy);
    return matrix;
}

} // namespace AEMath
} // namespace AbyssEngine
