#include "engine/math/BoundingSphere.h"
#include "engine/math/AEMath.h"

#include <new>









BoundingSphere::BoundingSphere(
    float x, float y, float z, float ex, float ey, float ez, float radius)
    : BoundingVolume(x, y, z, ex, ey, ez)
{
    this->radius = radius;
}

BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::update(float x, float y, float z)
{
    BoundingVolume::update(x, y, z);
}

// The sphere's centre in world space is the volume's centre offset by its
// extents. The original compiler inlines this everywhere (no standalone
// symbol), so it is expressed inline at each use below.

// Project `position` onto this sphere's surface. Points outside the sphere are
// left untouched (zero result); points inside are pushed out to the surface.
Vector BoundingSphere::projectCollisionOnSurface(const Vector& position)
{
    Vector center = Vector{centerX + extentsX, centerY + extentsY, centerZ + extentsZ};
    Vector delta = center - position;
    float length = VectorLength(delta);

    if (length >= radius) {
        return Vector{0.0f, 0.0f, 0.0f};
    }
    return center - delta * (radius / length);
}

int BoundingSphere::outerCollide(float x, float y, float z)
{
    Vector delta = Vector{x, y, z}
        - Vector{centerX + extentsX, centerY + extentsY, centerZ + extentsZ};
    float distanceSq = VectorDot(delta, delta);
    return distanceSq < radius * radius ? 1 : 0;
}

Vector BoundingSphere::getCollisionNormal(const Vector& position)
{
    return VectorNormalize(
        Vector{centerX + extentsX, centerY + extentsY, centerZ + extentsZ} - position);
}

int BoundingSphere::collide(float x, float y, float z)
{
    if (outerCollide(x, y, z)) {
        return 1;
    }
    return BoundingVolume::collide(x, y, z);
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

// Centre + radius, zero extents (Globals geometry loading). The original has no
// dedicated centre+radius constructor (it inlines a forward to the full ctor),
// so this builds the sphere through the real seven-argument constructor.
extern "C" void BoundingSphere_ctor(void* self, float cx, float cy, float cz, float r)
{
    new (self) BoundingSphere(cx, cy, cz, 0.0f, 0.0f, 0.0f, r);
}
