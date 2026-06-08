#include "class.h"

typedef int (*CollideFn)(BoundingAAB *self, float x, float y, float z);

extern "C" int BoundingVolume_collide(BoundingAAB *self, float x, float y, float z);

int BoundingAAB::collide(float x, float y, float z)
{
    CollideFn fn = *(CollideFn *)((char *)*(void **)this + 0xc);
    if (fn(this, x, y, z) == 0) {
        return 0;
    }
    return BoundingVolume_collide(this, x, y, z);
}
