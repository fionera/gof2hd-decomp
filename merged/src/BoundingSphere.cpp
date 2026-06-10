#include "BoundingSphere.h"




// ---- _BoundingSphere_151d64.cpp ----
BoundingSphere::~BoundingSphere()
{
}

// ---- update_151e70.cpp ----
void BoundingSphere::update(float x, float y, float z)
{
    return BoundingVolume::update(x, y, z);
}

// ---- BoundingSphere_151d28.cpp ----
__attribute__((visibility("hidden"))) extern void *const g_BoundingSphere_vtbl;

BoundingSphere::BoundingSphere(
    float x, float y, float z, float ex, float ey, float ez, float radius)
    : BoundingVolume(x, y, z, ex, ey, ez)
{
    void *vt = (void *)((char *)g_BoundingSphere_vtbl + 8);
    f32(this, 0x38) = radius;
    pp(this, 0x0) = vt;
}

// ---- projectCollisionOnSurface_151e74.cpp ----
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

// ---- outerCollide_151da0.cpp ----
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

// ---- getCollisionNormal_151e10.cpp ----
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

// ---- collide_151d68.cpp ----
typedef bool (*CollisionTestFn)(BoundingSphere *self);

bool BoundingSphere::collide(float x, float y, float z)
{
    CollisionTestFn fn = *(CollisionTestFn *)((char *)pp(this, 0x0) + 0xc);
    if (fn(this)) {
        return true;
    }
    return BoundingVolume::collide(x, y, z);
}
