#include "class.h"

// Hidden PC-relative pointer whose target word holds the BoundingVolume vtable
// base (stored into *this, +8 to skip the RTTI header).
__attribute__((visibility("hidden"))) extern void *const g_BoundingVolume_vtbl;

BoundingVolume::BoundingVolume(float a, float b, float c, float d, float e, float f)
{
    void *vt = (void *)((char *)g_BoundingVolume_vtbl + 8);
    this->f_14 = d;
    this->f_18 = e;
    this->f_1c = f;
    this->f_0 = vt;
    this->f_4 = 0;
    this->f_8 = a;
    this->f_c = b;
    this->f_10 = c;
}
