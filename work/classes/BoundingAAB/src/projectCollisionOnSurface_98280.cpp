#include "class.h"

extern "C" float external_fabsf(float value);
extern "C" Vector AEMath_operator_sub(const Vector &a, const Vector &b);

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
