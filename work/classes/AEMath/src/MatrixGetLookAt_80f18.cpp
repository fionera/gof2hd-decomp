#include "class.h"

namespace AbyssEngine {
namespace AEMath {

void MatrixGetLookAt(Matrix &result, const Vector &position, const Vector &target, const Vector &up)
{
    Vector diff = position - target;
    Vector dir = VectorNormalize(diff);
    Vector cross = VectorCross(up, dir);
    Vector right = VectorNormalize(cross);
    cross = VectorCross(dir, right);

    result.m[0] = right.x;
    result.m[1] = cross.x;
    result.m[2] = dir.x;
    result.m[3] = position.x;
    result.m[4] = right.y;
    result.m[5] = cross.y;
    result.m[6] = dir.y;
    result.m[7] = position.y;
    result.m[8] = right.z;
    result.m[9] = cross.z;
    result.m[10] = dir.z;
    result.m[11] = position.z;
    result.m[12] = 1.0f;
    result.m[13] = 1.0f;
    result.m[14] = 1.0f;
}

} // namespace AEMath
} // namespace AbyssEngine
