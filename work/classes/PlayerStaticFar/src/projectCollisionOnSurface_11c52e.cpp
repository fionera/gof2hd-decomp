#include "class.h"

extern "C" Vector BoundingVolume_staticProjectCollisionOnSurface(
    const Vector &value, void *volumes);

Vector PlayerStaticFar::projectCollisionOnSurface(const Vector &value)
{
    void *volumes = this->f_130;
    if (volumes != 0) {
        return BoundingVolume_staticProjectCollisionOnSurface(value, volumes);
    }

    Vector r = {0.0f, 0.0f, 0.0f};
    return r;
}
