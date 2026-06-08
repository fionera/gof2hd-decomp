#include "class.h"

// Each child is a BoundingVolume* whose vtable slot at +8 takes (x,y,z) and
// returns nonzero on collision. collide forwards this->center to every child.
// Center is reloaded as a raw 3-word block each iteration (matches ldmia).
typedef int (*CollideFn)(void *self, uint32_t x, uint32_t y, uint32_t z);

int BoundingVolume::collide(float, float, float)
{
    Array<BoundingVolume *> *children = (Array<BoundingVolume *> *)this->f_4;
    if (children != 0) {
        for (uint32_t i = 0; i < children->length; i++) {
            void *child = children->data[i];
            CollideFn fn = *(CollideFn *)((char *)*(void **)child + 8);
            uint32_t *c = (uint32_t *)((char *)this + 0x8);
            if (fn(child, c[0], c[1], c[2]) != 0) {
                return 1;
            }
            children = (Array<BoundingVolume *> *)this->f_4;
        }
    }
    return 0;
}
