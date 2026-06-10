#ifndef GOF2_BOUNDINGVOLUME_H
#define GOF2_BOUNDINGVOLUME_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- BoundingVolume class.
// Top-level class. Uses AbyssEngine::AEMath::Vector and Array<BoundingVolume*>.
//
// Field layout:
//   0x00 void*                     vtable
//   0x04 Array<BoundingVolume*>*   children
//   0x08 Vector                    center  (x@0x08, y@0x0c, z@0x10)
//   0x14 Vector                    extents (x@0x14, y@0x18, z@0x1c)

struct BoundingVolume {
    void* field_0x0;                    // +0x0  vtable
    Array<BoundingVolume*>* field_0x4;  // +0x4  children
    float field_0x8;                    // +0x8  center.x
    float field_0xc;                    // +0xc  center.y
    float field_0x10;                   // +0x10 center.z
    float field_0x14;                   // +0x14 extents.x
    float field_0x18;                   // +0x18 extents.y
    float field_0x1c;                   // +0x1c extents.z

    BoundingVolume(float cx, float cy, float cz, float ex, float ey, float ez);
    ~BoundingVolume();
    int collide(float x, float y, float z);
    void getCollisionNormal(const Vector &out);
    void setVolume(BoundingVolume *src);
    void staticProjectCollisionOnSurface(const Vector &v, Array<BoundingVolume *> *vols);
    void update(float x, float y, float z);
    Vector getProjectionVector(const Vector &v);
};
#endif
