#ifndef GOF2_BOUNDINGVOLUME_H
#define GOF2_BOUNDINGVOLUME_H

#include <cstdint>
#include <vector>

#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"

// Galaxy on Fire 2 -- BoundingVolume class.
// A volume node carrying a center/extents pair plus an owning list of child
// volumes. Children are polled through their virtual interface for collision,
// projection and update.

class BoundingVolume {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    // Real C++ polymorphic base: the dispatch table is the compiler-managed
    // vptr at offset 0 (it replaces the former hand-installed `void* vtable`
    // member, same 8-byte slot). Subclasses (BoundingSphere/BoundingAAB)
    // override the virtual collision interface; callers use named virtual calls
    // instead of reading the table by byte offset.
    Array<BoundingVolume*>* children;
    float centerX;
    float centerY;
    float centerZ;
    float extentsX;
    float extentsY;
    float extentsZ;

    BoundingVolume(float cx, float cy, float cz, float ex, float ey, float ez);
    virtual ~BoundingVolume();

    // Virtual collision interface (binary vtable slots collide@+0x8,
    // outerCollide@+0xc, update@+0x4, projectCollisionOnSurface@+0x10). The base
    // is the composite node: it polls its children through the same virtuals.
    virtual int collide(float x, float y, float z);
    virtual int outerCollide(float x, float y, float z);
    virtual void update(float x, float y, float z);
    virtual Vector projectCollisionOnSurface(const Vector& point);

    // getCollisionNormal is not polymorphically dispatched (subclasses give it a
    // different signature), so it stays a plain method. The base composite has no
    // surface of its own, so it returns a zero normal (BoundingAAB matches).
    Vector getCollisionNormal(const Vector& position);
    void setVolume(BoundingVolume* src);
    // Append this volume to the destination array. Both spellings forward to
    // the array's add.
    void setVolume_tail(Array<BoundingVolume*>* arr);
    void setArr(Array<BoundingVolume*>* arr);
    void staticProjectCollisionOnSurface(const Vector& v, Array<BoundingVolume*>* vols);
    Vector getProjectionVector(const Vector& v);
};

#endif
