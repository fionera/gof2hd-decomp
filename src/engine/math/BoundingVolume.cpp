#include "gof2/engine/math/BoundingVolume.h"
#include "gof2/engine/math/AEMath.h"

__attribute__((visibility("hidden"))) extern void *const g_BoundingVolume_vtbl;

BoundingVolume::~BoundingVolume()
{
    this->vtable = (void *)((char *)g_BoundingVolume_vtbl + 8);
    Array<BoundingVolume*> *children = this->children;
    if (children != 0) {
        // owning array: delete each child pointee, then free the array itself
        for (uint32_t i = 0; i < children->size(); i++) {
            delete (*children)[i];
        }
        children->clear();
        delete this->children;
    }
    this->children = 0;
}

// Each child is a BoundingVolume* whose vtable slot at +8 takes (x,y,z) and
// returns nonzero on collision. collide forwards this->center to every child.
typedef int (*CollideFn)(void *self, float x, float y, float z);

int BoundingVolume::collide(float, float, float)
{
    Array<BoundingVolume *> *children = this->children;
    if (children != 0) {
        for (uint32_t i = 0; i < children->size(); i++) {
            void *child = children->data()[i];
            CollideFn fn = *(CollideFn *)((char *)*(void **)child + 8);
            if (fn(child, this->centerX, this->centerY, this->centerZ) != 0) {
                return 1;
            }
            children = this->children;
        }
    }
    return 0;
}

// getCollisionNormal(Vector&): zero the three components of the passed vector.
void BoundingVolume::getCollisionNormal(const Vector &out)
{
    Vector *v = (Vector *)&out;
    v->x = 0.0f;
    v->y = 0.0f;
    v->z = 0.0f;
}

// Tail-call: copy the source volume's children into the new array.
void BoundingVolume::setVolume(BoundingVolume *src)
{
    Array<BoundingVolume*> *arr = new Array<BoundingVolume*>();
    this->children = arr;
    return ((BoundingVolume *)(src))->setVolume_tail(arr);
}

// Tail target of setVolume() (veneer 0x1ac218 -> Array<BoundingVolume*>::add at
// 0xefbc4): append `this` to the destination array.
void BoundingVolume::setVolume_tail(Array<BoundingVolume *> *arr)
{
    arr->push_back(this);
}

// Identical registration used by PlayerFixedObject::setBV (same Array::add target).
void BoundingVolume::setArr(Array<BoundingVolume *> *arr)
{
    arr->push_back(this);
}

BoundingVolume::BoundingVolume(float cx, float cy, float cz, float ex, float ey, float ez)
{
    void *vt = (void *)((char *)g_BoundingVolume_vtbl + 8);
    this->extentsX = ex;
    this->extentsY = ey;
    this->extentsZ = ez;
    this->vtable = vt;
    this->children = 0;
    this->centerX = cx;
    this->centerY = cy;
    this->centerZ = cz;
}

// child->vtable[0xc](x,y,z) -> bool collides; child->vtable[0x10](&out, child, this).
typedef int (*HitFn)(void *self, float x, float y, float z);
typedef void (*ProjFn)(Vector *out, void *self, void *bv);

// staticProjectCollisionOnSurface(v, vols): seed this->center with v, then run
// two passes projecting the point onto every colliding volume's surface.
void BoundingVolume::staticProjectCollisionOnSurface(const Vector &v, Array<BoundingVolume *> *vols)
{
    // this->center = v
    this->centerX = v.x;
    this->centerY = v.y;
    this->centerZ = v.z;

    if (vols != 0) {
        for (int pass = 0; pass != 2; pass++) {
            for (uint32_t i = 0; i < vols->size(); i++) {
                void *bv = vols->data()[i];
                HitFn hit = *(HitFn *)((char *)*(void **)bv + 0xc);
                if (hit(bv, this->centerX, this->centerY, this->centerZ) != 0) {
                    void *bv2 = vols->data()[i];
                    ProjFn proj = *(ProjFn *)((char *)*(void **)bv2 + 0x10);
                    Vector out;
                    proj(&out, bv2, this);
                    *(Vector *)&this->centerX = out;
                }
            }
        }
    }
}

// Each child's vtable slot at +4 takes (x,y,z); update forwards its params to
// every child, then stores them as this->center.
typedef void (*UpdateFn)(void *self, float x, float y, float z);

void BoundingVolume::update(float x, float y, float z)
{
    Array<BoundingVolume *> *children = this->children;
    if (children != 0) {
        for (uint32_t i = 0; i < children->size(); i++) {
            void *child = children->data()[i];
            UpdateFn fn = *(UpdateFn *)((char *)*(void **)child + 4);
            fn(child, x, y, z);
            children = this->children;
        }
    }
    Vector v;
    v.x = x;
    v.y = y;
    v.z = z;
    *(Vector *)&this->centerX = v;
}

// getProjectionVector(v): returns normalize(v - this->center).
Vector BoundingVolume::getProjectionVector(const Vector &v)
{
    Vector ret = AbyssEngine::AEMath::operator-(v, *(const Vector *)&this->centerX);
    ret = AbyssEngine::AEMath::VectorNormalize(ret);
    return ret;
}
