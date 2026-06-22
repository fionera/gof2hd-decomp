#ifndef GOF2_BOUNDINGVOLUME_H
#define GOF2_BOUNDINGVOLUME_H

#include "Vector.h"
#include "engine/core/Array.h"

class BoundingVolume {
public:

    // Real C++ polymorphic base: the dispatch table is the compiler-managed
    // vptr at offset 0 (it replaces the former hand-installed `void* vtable`
    // member, same 8-byte slot). Subclasses (BoundingSphere/BoundingAAB)
    // override the virtual collision interface; callers use named virtual calls
    // instead of reading the table by byte offset.
    Array<BoundingVolume *> *children;
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

    // Abstract surface projection: the base node never owns a surface of its
    // own, so the slot is pure -- only the leaf subclasses (BoundingSphere /
    // BoundingAAB) carry real geometry and emit a body. This makes
    // BoundingVolume an abstract base (it is never instantiated directly; the
    // binary has no BoundingVolume::projectCollisionOnSurface symbol, hence no
    // complete-object ctor C1 / deleting dtor D0 either).
    virtual AbyssEngine::AEMath::Vector projectCollisionOnSurface(const AbyssEngine::AEMath::Vector &point) = 0;

    // getCollisionNormal is not polymorphically dispatched (subclasses give it a
    // different signature), so it stays a plain method. The base composite has no
    // surface of its own, so it returns a zero normal (BoundingAAB matches).
    AbyssEngine::AEMath::Vector getCollisionNormal(const AbyssEngine::AEMath::Vector &position);

    void setVolume(BoundingVolume *src);

    // Store the child-volume array directly (str r1,[r0,#4] -> children).
    void setVolumes(Array<BoundingVolume *> *arr);

    void staticProjectCollisionOnSurface(const AbyssEngine::AEMath::Vector &v, Array<BoundingVolume *> *vols);

    AbyssEngine::AEMath::Vector getProjectionVector(const AbyssEngine::AEMath::Vector &v);
};

#endif
