#include "class.h"

extern "C" void AEGeometry_setPosition(void *geometry, const Vector &position);

typedef void (*BoundingVolumeSetPositionFn)(void *volume, float x, float y, float z);

void PlayerStation::setPosition(const Vector &position)
{
    F<float>(this, 0x58) = position.x;
    F<float>(this, 0x5c) = position.y;
    F<float>(this, 0x60) = position.z;
    AEGeometry_setPosition(P(this, 0x140), position);

    void *volumes = P(this, 0x130);
    if (volumes != 0) {
        for (uint32_t i = 0; i < F<uint32_t>(volumes, 0x0); ++i) {
            void *volume = *(void **)((char *)P(volumes, 0x4) + i * 4);
            BoundingVolumeSetPositionFn fn =
                *(BoundingVolumeSetPositionFn *)((char *)P(volume, 0x0) + 0x4);
            fn(volume, position.x, position.y, position.z);
            volumes = P(this, 0x130);
        }
    }
}
