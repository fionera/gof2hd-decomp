#include "class.h"

__attribute__((visibility("hidden"))) extern void *const g_BoundingSphere_vtbl;

BoundingSphere::BoundingSphere(
    float x, float y, float z, float ex, float ey, float ez, float radius)
    : BoundingVolume(x, y, z, ex, ey, ez)
{
    void *vt = (void *)((char *)g_BoundingSphere_vtbl + 8);
    f32(this, 0x38) = radius;
    pp(this, 0x0) = vt;
}
