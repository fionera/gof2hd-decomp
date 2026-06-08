#include "class.h"

__attribute__((visibility("hidden"))) extern void *const g_BoundingVolume_vtbl;

extern "C" void ArrayReleaseClasses_BVPtr(void *arr);  // 0x757a8
extern "C" void *ArrayBV_dtor(void *arr);              // 0x757b4  Array<BoundingVolume*>::~Array
extern "C" void operator_delete(void *p);              // 0x6eb48

BoundingVolume::~BoundingVolume()
{
    pp(this, 0x0) = (void *)((char *)g_BoundingVolume_vtbl + 8);
    void *children = pp(this, 0x4);
    if (children != 0) {
        ArrayReleaseClasses_BVPtr(children);
        if (pp(this, 0x4) != 0) {
            operator_delete(ArrayBV_dtor(pp(this, 0x4)));
        }
    }
    pp(this, 0x4) = 0;
}
