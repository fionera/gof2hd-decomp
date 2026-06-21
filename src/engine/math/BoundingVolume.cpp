#include "engine/math/BoundingVolume.h"
#include "engine/math/AEMath.h"

namespace AEMath = AbyssEngine::AEMath;

BoundingVolume::~BoundingVolume()
{
    if (children != nullptr) {
        // owning array: delete each child pointee, then free the array itself
        for (BoundingVolume* child : *children) {
            delete child;
        }
        children->clear();
        delete children;
    }
    children = nullptr;
}

// Composite node: each child is polled through its own virtual collide().
int BoundingVolume::collide(float, float, float)
{
    if (children != nullptr) {
        for (BoundingVolume* child : *children) {
            if (child->collide(centerX, centerY, centerZ) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

// Base composite outerCollide: a plain volume has no outer surface of its own
// (binary BoundingVolume::outerCollide returns 0); leaf subclasses override.
int BoundingVolume::outerCollide(float, float, float)
{
    return 0;
}

// Base projectCollisionOnSurface is only reached on leaf subclasses (they
// override it); a bare composite node projects the point onto itself (identity).
BoundingVolume::Vector BoundingVolume::projectCollisionOnSurface(const Vector& point)
{
    return point;
}

BoundingVolume::Vector BoundingVolume::getCollisionNormal(const Vector& position)
{
    (void)position;
    Vector out;
    out.x = 0.0f;
    out.y = 0.0f;
    out.z = 0.0f;
    return out;
}

// Allocate a fresh child-volume array and register the source volume into it.
void BoundingVolume::setVolume(BoundingVolume* src)
{
    children = new Array<BoundingVolume*>();
    children->push_back(src);
}

// Store the child-volume array directly (the binary is a single str at +4).
void BoundingVolume::setVolumes(Array<BoundingVolume*>* arr)
{
    children = arr;
}

BoundingVolume::BoundingVolume(float cx, float cy, float cz, float ex, float ey, float ez)
{
    children = nullptr;
    centerX = cx;
    centerY = cy;
    centerZ = cz;
    extentsX = ex;
    extentsY = ey;
    extentsZ = ez;
}

// Seed this->center with v, then run two passes projecting the point onto every
// colliding volume's surface, dispatched through each volume's virtuals.
void BoundingVolume::staticProjectCollisionOnSurface(const Vector& v, Array<BoundingVolume*>* vols)
{
    centerX = v.x;
    centerY = v.y;
    centerZ = v.z;

    if (vols != nullptr) {
        for (int pass = 0; pass != 2; pass++) {
            for (BoundingVolume* bv : *vols) {
                if (bv->outerCollide(centerX, centerY, centerZ) != 0) {
                    Vector out = bv->projectCollisionOnSurface(Vector{centerX, centerY, centerZ});
                    centerX = out.x;
                    centerY = out.y;
                    centerZ = out.z;
                }
            }
        }
    }
}

void BoundingVolume::update(float x, float y, float z)
{
    if (children != nullptr) {
        for (BoundingVolume* child : *children) {
            child->update(x, y, z);
        }
    }
    centerX = x;
    centerY = y;
    centerZ = z;
}

// Returns normalize(v - this->center).
BoundingVolume::Vector BoundingVolume::getProjectionVector(const Vector& v)
{
    Vector center;
    center.x = centerX;
    center.y = centerY;
    center.z = centerZ;
    return AEMath::VectorNormalize(v - center);
}
