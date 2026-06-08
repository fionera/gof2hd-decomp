#include "class.h"

__attribute__((visibility("hidden"))) extern void *const g_BoundingVolume_vtbl;

extern "C" void ArrayReleaseClasses_BVPtr(void *arr);  // 0x757a8
extern "C" void *ArrayBV_dtor(void *arr);              // 0x757b4  Array<BoundingVolume*>::~Array
extern "C" void operator_delete(void *p);              // 0x6eb48

BoundingVolume::~BoundingVolume()
{
    this->f_0 = (void *)((char *)g_BoundingVolume_vtbl + 8);
    void *children = this->f_4;
    if (children != 0) {
        ArrayReleaseClasses_BVPtr(children);
        if (this->f_4 != 0) {
            operator_delete(ArrayBV_dtor(this->f_4));
        }
    }
    this->f_4 = 0;
}
