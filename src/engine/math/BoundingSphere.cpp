#include "gof2/engine/math/BoundingSphere.h"
#include <new>

// Helpers to view the inherited center/extents floats as Vectors.
static inline Vector *centerOf(BoundingVolume *v) { return (Vector *)&v->centerX; }
static inline Vector *extentsOf(BoundingVolume *v) { return (Vector *)&v->extentsX; }

// ---- _BoundingSphere_151d64.cpp ----
BoundingSphere::~BoundingSphere()
{
}

// ---- update_151e70.cpp ----
void BoundingSphere::update(float x, float y, float z)
{
    return BoundingVolume::update(x, y, z);
}

// ---- BoundingSphere_151d28.cpp ----
__attribute__((visibility("hidden"))) extern void *const g_BoundingSphere_vtbl;

BoundingSphere::BoundingSphere(
    float x, float y, float z, float ex, float ey, float ez, float radius)
    : BoundingVolume(x, y, z, ex, ey, ez)
{
    void *vt = (void *)((char *)g_BoundingSphere_vtbl + 8);
    field_0x38 = radius;
    vtable = vt;
}

BoundingSphere::BoundingSphere(float cx, float cy, float cz, float radius)
    : BoundingSphere(cx, cy, cz, 0.0f, 0.0f, 0.0f, radius)
{
}

// ---- projectCollisionOnSurface_151e74.cpp ----
Vector BoundingSphere::projectCollisionOnSurface(const Vector &position)
{
    Vector result;
    Vector delta;
    Vector center;
    Vector scaled;

    AEMath_VectorAdd(&center, centerOf(this), extentsOf(this));
    AEMath_VectorSub(&delta, &center, &position);
    float length = AEMath_VectorLength(&delta);
    float radius = field_0x38;

    if (length >= radius) {
        result.x = 0.0f;
        result.y = 0.0f;
        result.z = 0.0f;
    } else {
        AEMath_VectorAdd(&center, centerOf(this), extentsOf(this));
        AEMath_VectorScale(&scaled, radius / length, &delta);
        AEMath_VectorSub(&result, &center, &scaled);
    }

    return result;
}

// ---- outerCollide_151da0.cpp ----
bool BoundingSphere::outerCollide(float x, float y, float z)
{
    Vector delta;
    Vector point;
    Vector sum;

    point.x = x;
    point.y = y;
    point.z = z;

    AEMath_VectorAdd(&sum, centerOf(this), extentsOf(this));
    AEMath_VectorSub(&delta, &point, &sum);
    float distance = AEMath_VectorDot(&delta, &delta);
    float radius = field_0x38;
    return distance < radius * radius;
}

// ---- getCollisionNormal_151e10.cpp ----
Vector BoundingSphere::getCollisionNormal(const Vector &position)
{
    Vector result;
    Vector center;

    AEMath_VectorAdd(&center, centerOf(this), extentsOf(this));
    AEMath_VectorSub(&result, &center, &position);
    AEMath_VectorNormalize(&center, &result);
    AEMath_VectorAssign(&result, &center);

    return result;
}

// ---- collide_151d68.cpp ----
typedef bool (*CollisionTestFn)(BoundingSphere *self);

bool BoundingSphere::collide(float x, float y, float z)
{
    CollisionTestFn fn = *(CollisionTestFn *)((char *)vtable + 0xc);
    if (fn(this)) {
        return true;
    }
    return BoundingVolume::collide(x, y, z);
}

// ---- C-ABI shims (recovered) ----
// Flat constructor entry points used where a BoundingSphere is allocated raw
// (operator new) and constructed through a C call. They forward to the real
// in-class constructors.

// BoundingSphere_constructor(self, cx, cy, cz, radius, ex, ey, ez) — full form
// with explicit centre, radius and extents (PlayerStation collision volumes).
extern "C" void BoundingSphere_constructor(void *self, float cx, float cy, float cz,
                                           float radius, float ex, float ey, float ez)
{
    new (self) BoundingSphere(cx, cy, cz, ex, ey, ez, radius);
}

// BoundingSphere_ctor(self, cx, cy, cz, r) — centre + radius, zero extents
// (Globals geometry loading).
extern "C" void BoundingSphere_ctor(void *self, float cx, float cy, float cz, float r)
{
    new (self) BoundingSphere(cx, cy, cz, r);
}

// BoundingSphere_ctor_ca(bs) — default sphere (centre origin, zero radius); the
// Level asteroid builder allocates the sphere then update()s it later.
void BoundingSphere_ctor_ca(BoundingSphere *bs)
{
    new (bs) BoundingSphere(0.0f, 0.0f, 0.0f, 0.0f);
}
