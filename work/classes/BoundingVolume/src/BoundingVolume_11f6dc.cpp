#include "class.h"

// Hidden PC-relative pointer whose target word holds the BoundingVolume vtable
// base (stored into *this, +8 to skip the RTTI header).
__attribute__((visibility("hidden"))) extern void *const g_BoundingVolume_vtbl;

BoundingVolume::BoundingVolume(float a, float b, float c, float d, float e, float f)
{
    void *vt = (void *)((char *)g_BoundingVolume_vtbl + 8);
    f32(this, 0x14) = d;
    f32(this, 0x18) = e;
    f32(this, 0x1c) = f;
    pp(this, 0x0) = vt;
    u32(this, 0x4) = 0;
    f32(this, 0x8) = a;
    f32(this, 0xc) = b;
    f32(this, 0x10) = c;
}
