#include "gof2/engine/math/BoundingAAB.h"
#include "gof2/engine/math/BoundingVolume.h"

extern "C" float external_fabsf(float value);
extern "C" Vector AEMath_operator_sub(const Vector &a, const Vector &b);

int BoundingAAB::outerCollide(float x, float y, float z)
{
    float centerX = this->centerX + this->extentsX;
    float extentX = this->halfExtentX;
    if (!(centerX - extentX < x)) {
        return 0;
    }
    if (!(x < centerX + extentX)) {
        return 0;
    }

    float centerY = this->centerY + this->extentsY;
    float extentY = this->halfExtentY;
    if (!(centerY - extentY < y)) {
        return 0;
    }
    if (!(y < centerY + extentY)) {
        return 0;
    }

    float centerZ = this->centerZ + this->extentsZ;
    float extentZ = this->halfExtentZ;
    if (!(centerZ - extentZ < z)) {
        return 0;
    }
    if (z < centerZ + extentZ) {
        return 1;
    }
    return 0;
}

Vector BoundingAAB::projectCollisionOnSurface(const Vector &point)
{
    float distances[6];
    Vector offsets[6] = {};   // each candidate face-offset, only one axis non-zero

    float centerX = this->centerX + this->extentsX;
    float extentX = this->halfExtentX;
    float highX = centerX + extentX;
    float lowX = centerX - extentX;
    float pointX = point.x;
    offsets[0].x = pointX - highX;
    offsets[1].x = pointX - lowX;

    float centerY = this->centerY + this->extentsY;
    float extentY = this->halfExtentY;
    float highY = centerY + extentY;
    float lowY = centerY - extentY;
    float pointY = point.y;
    offsets[2].y = pointY - highY;
    offsets[3].y = pointY - lowY;

    float centerZ = this->centerZ + this->extentsZ;
    float extentZ = this->halfExtentZ;
    float highZ = centerZ + extentZ;
    float lowZ = centerZ - extentZ;
    float pointZ = point.z;
    offsets[4].z = pointZ - highZ;
    offsets[5].z = pointZ - lowZ;

    distances[0] = external_fabsf(offsets[0].x);
    distances[1] = external_fabsf(offsets[1].x);
    distances[2] = external_fabsf(offsets[2].y);
    distances[3] = external_fabsf(offsets[3].y);
    distances[4] = external_fabsf(offsets[4].z);
    distances[5] = external_fabsf(offsets[5].z);

    float closest = distances[0];
    int closestIndex = 0;
    for (int i = 1; i != 6; i++) {
        if (distances[i] < closest) {
            closest = distances[i];
            closestIndex = i;
        }
    }

    return AEMath_operator_sub(point, offsets[closestIndex]);
}

__attribute__((visibility("hidden"))) extern void *const g_BoundingAAB_vtbl;

BoundingAAB::BoundingAAB(float x, float y, float z, float ex, float ey, float ez,
                         float width, float height, float depth)
    : BoundingVolume(x, y, z, ex, ey, ez)
{

    float halfWidth = width * 0.5f;
    float extentX = width * -0.5f;
    if (0.0f < halfWidth) {
        extentX = halfWidth;
    }

    float halfHeight = height * 0.5f;
    float extentY = height * -0.5f;
    if (0.0f < halfHeight) {
        extentY = halfHeight;
    }

    float halfDepth = depth * 0.5f;
    float extentZ = depth * -0.5f;
    if (0.0f < halfDepth) {
        extentZ = halfDepth;
    }

    this->vtable = (char *)g_BoundingAAB_vtbl + 8;
    this->halfExtentX = extentX;
    this->halfExtentY = extentY;
    this->halfExtentZ = extentZ;
}

Vector BoundingAAB::getCollisionNormal(const Vector &)
{
    Vector out;
    out.x = 0.0f;
    out.y = 0.0f;
    out.z = 0.0f;
    return out;
}

typedef int (*CollideFn)(BoundingAAB *self, float x, float y, float z);

int BoundingAAB::collide(float x, float y, float z)
{
    CollideFn fn = *(CollideFn *)((char *)*(void **)this + 0xc);
    if (fn(this, x, y, z) == 0) {
        return 0;
    }
    return ((BoundingVolume *)(this))->collide(x, y, z);
}

void BoundingAAB::update(float x, float y, float z)
{
    return ((BoundingVolume *)(this))->update(x, y, z);
}
