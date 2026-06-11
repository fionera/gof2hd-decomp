#ifndef GOF2_BOUNDINGAAB_H
#define GOF2_BOUNDINGAAB_H
#include "gof2/common.h"
#include "gof2/BoundingVolume.h"
// Galaxy on Fire 2 -- BoundingAAB (axis-aligned box) bounding volume.
// Derives from BoundingVolume; adds per-axis half-extents at 0x20/0x24/0x28.
// Inherited layout (from BoundingVolume): vtable@0x0, children@0x4,
//   center@0x8/0xc/0x10, extents@0x14/0x18/0x1c.

struct BoundingAAB : BoundingVolume {
    float halfExtentX;                   // +0x20  box half-extent X
    float halfExtentY;                   // +0x24  box half-extent Y
    float halfExtentZ;                   // +0x28  box half-extent Z

    BoundingAAB(float x, float y, float z, float ex, float ey, float ez,
                float width, float height, float depth);
    int outerCollide(float x, float y, float z);
    Vector projectCollisionOnSurface(const Vector &point);
    Vector getCollisionNormal(const Vector &point);
    int collide(float x, float y, float z);
    void update(float x, float y, float z);
};
#endif
