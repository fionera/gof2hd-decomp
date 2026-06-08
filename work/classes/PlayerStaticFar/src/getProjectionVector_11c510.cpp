#include "class.h"

extern "C" Vector BoundingVolume_getProjectionVector(void *volume, const Vector &value);

Vector PlayerStaticFar::getProjectionVector(const Vector &value)
{
    void *volumes = this->f_130;
    if (volumes != 0) {
        void *items = volumes->f_4;
        void *volume = *(void **)items;
        return BoundingVolume_getProjectionVector(volume, value);
    }

    Vector r = {0.0f, 0.0f, 0.0f};
    return r;
}
