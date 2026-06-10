#include "BoundingAAB.h"


extern "C" float external_fabsf(float value);
extern "C" Vector AEMath_operator_sub(const Vector &a, const Vector &b);
extern "C" int BoundingVolume_collide(BoundingAAB *self, float x, float y, float z);
extern "C" void BoundingVolume_update(BoundingAAB *self, float x, float y, float z);

// ---- outerCollide_981d2.cpp ----
int BoundingAAB::outerCollide(float x, float y, float z)
{
    BoundingAAB *self = this;
    float centerX = F<float>(self, 0x8) + F<float>(self, 0x14);
    float extentX = F<float>(self, 0x20);
    if (!(centerX - extentX < x)) {
        return 0;
    }
    if (!(x < centerX + extentX)) {
        return 0;
    }

    float centerY = F<float>(self, 0xc) + F<float>(self, 0x18);
    float extentY = F<float>(self, 0x24);
    if (!(centerY - extentY < y)) {
        return 0;
    }
    if (!(y < centerY + extentY)) {
        return 0;
    }

    float centerZ = F<float>(self, 0x10) + F<float>(self, 0x1c);
    float extentZ = F<float>(self, 0x28);
    if (!(centerZ - extentZ < z)) {
        return 0;
    }
    if (z < centerZ + extentZ) {
        return 1;
    }
    return 0;
}

// ---- projectCollisionOnSurface_98280.cpp ----
Vector BoundingAAB::projectCollisionOnSurface(const Vector &point)
{
    float distances[6];
    char offsetStorage[72];
    Vector *offsets = (Vector *)offsetStorage;
    uint32_t zero = 0;

    float centerX = F<float>(this, 0x8) + F<float>(this, 0x14);
    float extentX = F<float>(this, 0x20);
    float highX = centerX + extentX;
    float lowX = centerX - extentX;
    float pointX = point.x;
    F<uint32_t>(&offsets[0], 0x4) = zero;
    F<uint32_t>(&offsets[0], 0x8) = zero;
    F<uint32_t>(&offsets[1], 0x4) = zero;
    F<uint32_t>(&offsets[1], 0x8) = zero;
    offsets[0].x = pointX - highX;
    offsets[1].x = pointX - lowX;

    float centerY = F<float>(this, 0xc) + F<float>(this, 0x18);
    float extentY = F<float>(this, 0x24);
    float highY = centerY + extentY;
    float lowY = centerY - extentY;
    float pointY = point.y;
    F<uint32_t>(&offsets[2], 0x0) = zero;
    F<uint32_t>(&offsets[2], 0x8) = zero;
    F<uint32_t>(&offsets[3], 0x0) = zero;
    F<uint32_t>(&offsets[3], 0x8) = zero;
    offsets[2].y = pointY - highY;
    offsets[3].y = pointY - lowY;

    float centerZ = F<float>(this, 0x10) + F<float>(this, 0x1c);
    float extentZ = F<float>(this, 0x28);
    float highZ = centerZ + extentZ;
    float lowZ = centerZ - extentZ;
    float pointZ = point.z;
    F<uint32_t>(&offsets[4], 0x0) = zero;
    F<uint32_t>(&offsets[4], 0x4) = zero;
    F<uint32_t>(&offsets[5], 0x0) = zero;
    F<uint32_t>(&offsets[5], 0x4) = zero;
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

// ---- BoundingAAB_98104.cpp ----
__attribute__((visibility("hidden"))) extern void *const g_BoundingAAB_vtbl;

BoundingAAB::BoundingAAB(float x, float y, float z, float ex, float ey, float ez,
                         float width, float height, float depth)
    : BoundingVolume(x, y, z, ex, ey, ez)
{
    BoundingAAB *self = this;

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

    F<void *>(self, 0x0) = (char *)g_BoundingAAB_vtbl + 8;
    F<float>(self, 0x20) = extentX;
    F<float>(self, 0x24) = extentY;
    F<float>(self, 0x28) = extentZ;
}

// ---- getCollisionNormal_98270.cpp ----
Vector BoundingAAB::getCollisionNormal(const Vector &)
{
    Vector out;
    out.x = 0.0f;
    out.y = 0.0f;
    out.z = 0.0f;
    return out;
}

// ---- collide_9819c.cpp ----
typedef int (*CollideFn)(BoundingAAB *self, float x, float y, float z);


int BoundingAAB::collide(float x, float y, float z)
{
    CollideFn fn = *(CollideFn *)((char *)*(void **)this + 0xc);
    if (fn(this, x, y, z) == 0) {
        return 0;
    }
    return BoundingVolume_collide(this, x, y, z);
}

// ---- update_9827a.cpp ----
void BoundingAAB::update(float x, float y, float z)
{
    return BoundingVolume_update(this, x, y, z);
}
