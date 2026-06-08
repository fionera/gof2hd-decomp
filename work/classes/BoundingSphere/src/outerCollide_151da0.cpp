#include "class.h"

bool BoundingSphere::outerCollide(float x, float y, float z)
{
    char delta[12];
    char sum[12];
    char pointBytes[12];
    Vector *point = (Vector *)(void *)sum;

    point->x = x;
    point->y = y;
    point->z = z;

    AEMath_VectorAdd(pointBytes, &vec(this, 0x8), &vec(this, 0x14));
    AEMath_VectorSub(delta, point, pointBytes);
    float distance = AEMath_VectorDot(delta, delta);
    float radius = f32(this, 0x38);
    return distance < radius * radius;
}
