#include "class.h"

namespace AbyssEngine {
namespace AEMath {

void BSphere::Merge(const BSphere &other)
{
    if (other.radius2 > radius2) {
        radius2 = other.radius2;
    }

    if (other.radius != 0.0f) {
        if (radius == 0.0f) {
            radius = other.radius;
            __builtin_memcpy(&center.x, &other.center.x, sizeof(center.x));
            __builtin_memcpy(&center.y, &other.center.y, sizeof(center.y));
            __builtin_memcpy(&center.z, &other.center.z, sizeof(center.z));
            __builtin_memcpy(&radius2, &other.radius2, sizeof(radius2));
            operator=(other);
        } else {
            float dy = other.center.y - center.y;
            float dx = other.center.x - center.x;
            float dz = other.center.z - center.z;
            float distance2 = dy * dy + dx * dx + dz * dz;
            float distance = sqrtf(distance2);

            float otherRadius = other.radius;
            float thisRadius = radius;
            if (distance == 0.0f) {
                if (thisRadius > otherRadius) {
                    otherRadius = thisRadius;
                }
            } else {
                float distancePlusOther = distance + otherRadius;
                if (thisRadius > distancePlusOther) {
                    return;
                }
                if (distance - otherRadius < -thisRadius) {
                    __builtin_memcpy(&center.x, &other.center.x, sizeof(center.x));
                    __builtin_memcpy(&center.y, &other.center.y, sizeof(center.y));
                    __builtin_memcpy(&center.z, &other.center.z, sizeof(center.z));
                    radius = otherRadius;
                    return;
                }

                float scale = ((distancePlusOther - thisRadius) * 0.5f) / distance;
                center.y = center.y + dy * scale;
                center.z = center.z + dz * scale;
                otherRadius = (distance + thisRadius + otherRadius) * 0.5f;
                center.x = center.x + dx * scale;
            }
            radius = otherRadius;
        }
    }
}

} // namespace AEMath
} // namespace AbyssEngine
