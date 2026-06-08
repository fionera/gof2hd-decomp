#include "class.h"

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
