#include "class.h"

extern "C" Vector BoundingVolume_getProjectionVector(void *volume, const Vector &position);

Vector PlayerStation::getProjectionVector(const Vector &position)
{
    void *volumes = P(this, 0x130);
    if (volumes != 0) {
        void *items = P(volumes, 0x4);
        uint32_t index = F<uint32_t>(this, 0x150);
        void *volume = *(void **)((char *)items + index * 4);
        return BoundingVolume_getProjectionVector(volume, position);
    }

    Vector result = {0.0f, 0.0f, 0.0f};
    return result;
}
