#include "engine/math/BoundingVolume.h"
#include "engine/math/AEMath.h"

namespace AEMath = AbyssEngine::AEMath;

__attribute__((visibility("hidden"))) extern void* const g_BoundingVolume_vtbl;

BoundingVolume::~BoundingVolume()
{
    vtable = (char*)g_BoundingVolume_vtbl + 8;
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

// Each child is polled through its own virtual interface; we read the child's
// dispatch table directly at the documented slot offset.
typedef int (*CollideFn)(void* self, float x, float y, float z);

int BoundingVolume::collide(float, float, float)
{
    if (children != nullptr) {
        for (BoundingVolume* child : *children) {
            CollideFn fn = *(CollideFn*)((char*)*(void**)child + 8);
            if (fn(child, centerX, centerY, centerZ) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

void BoundingVolume::getCollisionNormal(Vector& out)
{
    out.x = 0.0f;
    out.y = 0.0f;
    out.z = 0.0f;
}

// Copy the source volume's children into a freshly allocated array.
void BoundingVolume::setVolume(BoundingVolume* src)
{
    children = new std::vector<BoundingVolume*>();
    src->setVolume_tail(children);
}

// Append this volume to the destination array.
void BoundingVolume::setVolume_tail(std::vector<BoundingVolume*>* arr)
{
    arr->push_back(this);
}

// Identical registration used by PlayerFixedObject::setBV.
void BoundingVolume::setArr(std::vector<BoundingVolume*>* arr)
{
    arr->push_back(this);
}

BoundingVolume::BoundingVolume(float cx, float cy, float cz, float ex, float ey, float ez)
{
    vtable = (char*)g_BoundingVolume_vtbl + 8;
    children = nullptr;
    centerX = cx;
    centerY = cy;
    centerZ = cz;
    extentsX = ex;
    extentsY = ey;
    extentsZ = ez;
}

// Child collision/projection dispatch: slot +0xc tests collision, slot +0x10
// projects this volume's point onto the colliding surface.
typedef int (*HitFn)(void* self, float x, float y, float z);
typedef void (*ProjFn)(BoundingVolume::Vector* out, void* self, void* bv);

// Seed this->center with v, then run two passes projecting the point onto every
// colliding volume's surface.
void BoundingVolume::staticProjectCollisionOnSurface(const Vector& v, std::vector<BoundingVolume*>* vols)
{
    centerX = v.x;
    centerY = v.y;
    centerZ = v.z;

    if (vols != nullptr) {
        for (int pass = 0; pass != 2; pass++) {
            for (BoundingVolume* bv : *vols) {
                HitFn hit = *(HitFn*)((char*)*(void**)bv + 0xc);
                if (hit(bv, centerX, centerY, centerZ) != 0) {
                    ProjFn proj = *(ProjFn*)((char*)*(void**)bv + 0x10);
                    Vector out;
                    proj(&out, bv, this);
                    centerX = out.x;
                    centerY = out.y;
                    centerZ = out.z;
                }
            }
        }
    }
}

// Child update dispatch: slot +4 forwards the new position to every child.
typedef void (*UpdateFn)(void* self, float x, float y, float z);

void BoundingVolume::update(float x, float y, float z)
{
    if (children != nullptr) {
        for (BoundingVolume* child : *children) {
            UpdateFn fn = *(UpdateFn*)((char*)*(void**)child + 4);
            fn(child, x, y, z);
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
