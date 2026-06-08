#include "class.h"

Vector BoundingSphere::projectCollisionOnSurface(const Vector &position)
{
    Vector result;
    char delta[12];
    char center[12];
    char scaled[12];

    AEMath_VectorAdd(center, &vec(this, 0x8), &vec(this, 0x14));
    AEMath_VectorSub(delta, center, &position);
    float length = AEMath_VectorLength(delta);
    float radius = f32(this, 0x38);

    if (length >= radius) {
        result.x = 0.0f;
        result.y = 0.0f;
        result.z = 0.0f;
    } else {
        AEMath_VectorAdd(center, &vec(this, 0x8), &vec(this, 0x14));
        AEMath_VectorScale(scaled, radius / length, delta);
        AEMath_VectorSub(&result, center, scaled);
    }

    return result;
}
