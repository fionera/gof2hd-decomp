#include "class.h"

typedef bool (*CollisionTestFn)(BoundingSphere *self);

bool BoundingSphere::collide(float x, float y, float z)
{
    CollisionTestFn fn = *(CollisionTestFn *)((char *)pp(this, 0x0) + 0xc);
    if (fn(this)) {
        return true;
    }
    return BoundingVolume::collide(x, y, z);
}
