#include "class.h"

Vector BoundingSphere::getCollisionNormal(const Vector &position)
{
    Vector result;
    char center[12];
    void *centerPtr = center;
    const void *positionPtr = &position;

    AEMath_VectorAdd(centerPtr, &vec(this, 0x8), &vec(this, 0x14));
    AEMath_VectorSub(&result, centerPtr, positionPtr);
    AEMath_VectorNormalize(centerPtr, &result);
    AEMath_VectorAssign(&result, centerPtr);

    return result;
}
