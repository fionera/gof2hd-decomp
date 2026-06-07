#include "class.h"

extern "C" Vector BoundingVolume_staticProjectCollisionOnSurface(
    const Vector &position, void *volumes);

Vector PlayerStation::projectCollisionOnSurface(const Vector &position)
{
    return BoundingVolume_staticProjectCollisionOnSurface(position, P(this, 0x130));
}
