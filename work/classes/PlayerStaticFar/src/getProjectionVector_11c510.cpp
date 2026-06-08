#include "class.h"

extern "C" Vector BoundingVolume_getProjectionVector(void *volume, const Vector &value);

Vector PlayerStaticFar::getProjectionVector(const Vector &value)
{
    void *volumes = pp(this, 0x130);
    if (volumes != 0) {
        void *items = pp(volumes, 0x4);
        void *volume = *(void **)items;
        return BoundingVolume_getProjectionVector(volume, value);
    }

    Vector r = {0.0f, 0.0f, 0.0f};
    return r;
}
