#include "gof2/engine/math/BoundingSphere.h"
#include "gof2/engine/math/AEMath.h"

#include <new>

// The recovered per-class vtable pointer the constructor installs over the one
// the BoundingVolume base writes.
__attribute__((visibility("hidden"))) extern void* const g_BoundingSphere_vtbl;

using AbyssEngine::AEMath::Vector;
using AbyssEngine::AEMath::VectorDot;
using AbyssEngine::AEMath::VectorLength;
using AbyssEngine::AEMath::VectorNormalize;
using AbyssEngine::AEMath::operator+;
using AbyssEngine::AEMath::operator-;
using AbyssEngine::AEMath::operator*;

BoundingSphere::BoundingSphere(
    float x, float y, float z, float ex, float ey, float ez, float radius)
    : BoundingVolume(x, y, z, ex, ey, ez)
{
    this->radius = radius;
    this->vtable = (void*)((char*)g_BoundingSphere_vtbl + 8);
}

BoundingSphere::BoundingSphere(float cx, float cy, float cz, float radius)
    : BoundingSphere(cx, cy, cz, 0.0f, 0.0f, 0.0f, radius)
{
}

BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::update(float x, float y, float z)
{
    BoundingVolume::update(x, y, z);
}

// The sphere's centre in world space is the volume's centre offset by its
// extents.
Vector BoundingSphere::worldCenter() const
{
    return Vector{centerX + extentsX, centerY + extentsY, centerZ + extentsZ};
}

// Project `position` onto this sphere's surface. Points outside the sphere are
// left untouched (zero result); points inside are pushed out to the surface.
Vector BoundingSphere::projectCollisionOnSurface(const Vector& position)
{
    Vector center = worldCenter();
    Vector delta = center - position;
    float length = VectorLength(delta);

    if (length >= radius) {
        return Vector{0.0f, 0.0f, 0.0f};
    }
    return center - delta * (radius / length);
}

bool BoundingSphere::outerCollide(float x, float y, float z)
{
    Vector delta = Vector{x, y, z} - worldCenter();
    float distanceSq = VectorDot(delta, delta);
    return distanceSq < radius * radius;
}

Vector BoundingSphere::getCollisionNormal(const Vector& position)
{
    return VectorNormalize(worldCenter() - position);
}

bool BoundingSphere::collide(float x, float y, float z)
{
    if (outerCollide(x, y, z)) {
        return true;
    }
    return BoundingVolume::collide(x, y, z) != 0;
}

// ---- Flat construction entry points ----
// Used where a BoundingSphere is allocated raw (operator new) and constructed
// through a C call. They forward to the real in-class constructors.

// Full form with explicit centre, radius and extents (PlayerStation collision
// volumes).
extern "C" void BoundingSphere_constructor(void* self, float cx, float cy, float cz,
                                           float radius, float ex, float ey, float ez)
{
    new (self) BoundingSphere(cx, cy, cz, ex, ey, ez, radius);
}

// Centre + radius, zero extents (Globals geometry loading).
extern "C" void BoundingSphere_ctor(void* self, float cx, float cy, float cz, float r)
{
    new (self) BoundingSphere(cx, cy, cz, r);
}

// Default sphere (centre origin, zero radius); the Level asteroid builder
// allocates the sphere then update()s it later.
void BoundingSphere_ctor_ca(BoundingSphere* bs)
{
    new (bs) BoundingSphere(0.0f, 0.0f, 0.0f, 0.0f);
}
