#include "class.h"

extern "C" float (*MatrixSetRotation_809f0_sinf)(float) = ::sinf;
extern "C" float (*MatrixSetRotation_809f0_cosf)(float) = ::cosf;

namespace AbyssEngine {
namespace AEMath {

Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z, RotationOrder order)
{
    float (*sinFunc)(float) = MatrixSetRotation_809f0_sinf;
    float sx = sinFunc(x);
    float (*cosFunc)(float) = MatrixSetRotation_809f0_cosf;
    float sy = sinFunc(y);
    float sz = sinFunc(z);
    float cx = cosFunc(x);
    float cy = cosFunc(y);
    float cz = cosFunc(z);

    switch (order) {
    case ROTATION_ORDER_XYZ:
        matrix.m[0] = cy * cz;
        matrix.m[1] = -(sz * cy);
        matrix.m[2] = sy;
        matrix.m[8] = sx * sz - sy * cx * cz;
        matrix.m[9] = sx * cz + sz * sy * cx;
        matrix.m[10] = cx * cy;
        matrix.m[4] = sy * sx * cz + sz * cx;
        matrix.m[5] = cx * cz - sx * sy * sz;
        matrix.m[6] = -(sx * cy);
        break;

    case ROTATION_ORDER_XZY:
        matrix.m[0] = cy * cz;
        matrix.m[1] = -sz;
        matrix.m[2] = sy * cz;
        matrix.m[4] = sx * sy + sz * cx * cy;
        matrix.m[5] = cx * cz;
        matrix.m[6] = sz * sy * cx - sx * cy;
        matrix.m[8] = sz * sx * cy - sy * cx;
        matrix.m[9] = sx * cz;
        matrix.m[10] = cx * cy + sx * sy * sz;
        break;

    case ROTATION_ORDER_YXZ:
        matrix.m[4] = sz * cx;
        matrix.m[5] = cx * cz;
        matrix.m[6] = -sx;
        matrix.m[0] = cy * cz + sx * sy * sz;
        matrix.m[1] = sy * sx * cz - sz * cy;
        matrix.m[2] = sy * cx;
        matrix.m[8] = sz * sx * cy - sy * cz;
        matrix.m[9] = sy * sz + sx * cy * cz;
        matrix.m[10] = cx * cy;
        break;

    case ROTATION_ORDER_YZX:
        matrix.m[4] = sz;
        matrix.m[5] = cx * cz;
        matrix.m[6] = -(sx * cz);
        matrix.m[8] = -(sy * cz);
        matrix.m[9] = sx * cy + cx * sy * sz;
        matrix.m[10] = cx * cy - sx * sy * sz;
        matrix.m[0] = cy * cz;
        matrix.m[1] = sx * sy - cx * sz * cy;
        matrix.m[2] = sx * sz * cy + sy * cx;
        break;

    case ROTATION_ORDER_ZXY:
        matrix.m[8] = -(sy * cx);
        matrix.m[9] = sx;
        matrix.m[10] = cx * cy;
        matrix.m[0] = cy * cz - sx * sy * sz;
        matrix.m[1] = -(sz * cx);
        matrix.m[2] = sy * cz + sx * sz * cy;
        matrix.m[4] = sy * sx * cz + sz * cy;
        matrix.m[5] = cx * cz;
        matrix.m[6] = sy * sz - sx * cy * cz;
        break;

    case ROTATION_ORDER_ZYX:
        matrix.m[8] = -sy;
        matrix.m[9] = sx * cy;
        matrix.m[10] = cx * cy;
        matrix.m[4] = sz * cy;
        matrix.m[5] = cx * cz + sx * sy * sz;
        matrix.m[6] = cx * sy * sz - sx * cz;
        matrix.m[0] = cy * cz;
        matrix.m[1] = sy * sx * cz - sz * cx;
        matrix.m[2] = sy * cx * cz + sx * sz;
        break;
    }

    return matrix;
}

} // namespace AEMath
} // namespace AbyssEngine
