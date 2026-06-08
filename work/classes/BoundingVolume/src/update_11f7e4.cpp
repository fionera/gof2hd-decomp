#include "class.h"

extern "C" void AEMath_Vector_assign(Vector *dst, const Vector *src);  // 0x6eb3c

// Each child's vtable slot at +4 takes (x,y,z); update forwards its params to
// every child, then stores them as this->center.
typedef void (*UpdateFn)(void *self, float x, float y, float z);

void BoundingVolume::update(float x, float y, float z)
{
    Array<BoundingVolume *> *children = (Array<BoundingVolume *> *)this->f_4;
    if (children != 0) {
        for (uint32_t i = 0; i < children->length; i++) {
            void *child = children->data[i];
            UpdateFn fn = *(UpdateFn *)((char *)*(void **)child + 4);
            fn(child, x, y, z);
            children = (Array<BoundingVolume *> *)this->f_4;
        }
    }
    Vector v;
    v.x = x;
    v.y = y;
    v.z = z;
    AEMath_Vector_assign((Vector *)((char *)this + 8), &v);
}
