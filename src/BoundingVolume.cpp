#include "gof2/BoundingVolume.h"


// ---- _BoundingVolume_11f718.cpp ----
__attribute__((visibility("hidden"))) extern void *const g_BoundingVolume_vtbl;

extern "C" void ArrayReleaseClasses_BVPtr(void *arr);  // 0x757a8
extern "C" void *ArrayBV_dtor(void *arr);              // 0x757b4  Array<BoundingVolume*>::~Array
extern "C" void operator_delete(void *p);              // 0x6eb48

BoundingVolume::~BoundingVolume()
{
    this->field_0x0 = (void *)((char *)g_BoundingVolume_vtbl + 8);
    Array<BoundingVolume*> *children = this->field_0x4;
    if (children != 0) {
        ArrayReleaseClasses_BVPtr(children);
        if (this->field_0x4 != 0) {
            operator_delete(ArrayBV_dtor(this->field_0x4));
        }
    }
    this->field_0x4 = 0;
}

// ---- collide_11f870.cpp ----
// Each child is a BoundingVolume* whose vtable slot at +8 takes (x,y,z) and
// returns nonzero on collision. collide forwards this->center to every child.
typedef int (*CollideFn)(void *self, float x, float y, float z);

int BoundingVolume::collide(float, float, float)
{
    Array<BoundingVolume *> *children = this->field_0x4;
    if (children != 0) {
        for (uint32_t i = 0; i < children->size(); i++) {
            void *child = children->data()[i];
            CollideFn fn = *(CollideFn *)((char *)*(void **)child + 8);
            if (fn(child, this->field_0x8, this->field_0xc, this->field_0x10) != 0) {
                return 1;
            }
            children = this->field_0x4;
        }
    }
    return 0;
}

// ---- getCollisionNormal_11f7d8.cpp ----
// getCollisionNormal(Vector&): zero the three components of the passed vector.
void BoundingVolume::getCollisionNormal(const Vector &out)
{
    Vector *v = (Vector *)&out;
    v->x = 0.0f;
    v->y = 0.0f;
    v->z = 0.0f;
}

// ---- setVolume_11f750.cpp ----
extern "C" void *operator_new(uint32_t size);                       // 0x6eb24
extern "C" void ArrayBV_ctor(void *a);                             // 0x730c0  Array<BoundingVolume*>::Array()
// Tail-call: copy the source volume's children into the new array.
extern "C" void BoundingVolume_setVolume_tail(BoundingVolume *src, void *arr);  // 0x1ac218

void BoundingVolume::setVolume(BoundingVolume *src)
{
    Array<BoundingVolume*> *arr = new Array<BoundingVolume*>();
    this->field_0x4 = arr;
    return BoundingVolume_setVolume_tail(src, arr);
}

// ---- BoundingVolume_11f6dc.cpp ----
BoundingVolume::BoundingVolume(float cx, float cy, float cz, float ex, float ey, float ez)
{
    void *vt = (void *)((char *)g_BoundingVolume_vtbl + 8);
    this->field_0x14 = ex;
    this->field_0x18 = ey;
    this->field_0x1c = ez;
    this->field_0x0 = vt;
    this->field_0x4 = 0;
    this->field_0x8 = cx;
    this->field_0xc = cy;
    this->field_0x10 = cz;
}

// ---- staticProjectCollisionOnSurface_11f8b0.cpp ----
extern "C" void AEMath_Vector_assign(Vector *dst, const Vector *src);  // 0x6eb3c

// child->vtable[0xc](x,y,z) -> bool collides; child->vtable[0x10](&out, child, this).
typedef int (*HitFn)(void *self, float x, float y, float z);
typedef void (*ProjFn)(Vector *out, void *self, void *bv);

// staticProjectCollisionOnSurface(v, vols): seed this->center with v, then run
// two passes projecting the point onto every colliding volume's surface.
void BoundingVolume::staticProjectCollisionOnSurface(const Vector &v, Array<BoundingVolume *> *vols)
{
    // this->center = v
    this->field_0x8 = v.x;
    this->field_0xc = v.y;
    this->field_0x10 = v.z;

    if (vols != 0) {
        for (int pass = 0; pass != 2; pass++) {
            for (uint32_t i = 0; i < vols->size(); i++) {
                void *bv = vols->data()[i];
                HitFn hit = *(HitFn *)((char *)*(void **)bv + 0xc);
                if (hit(bv, this->field_0x8, this->field_0xc, this->field_0x10) != 0) {
                    void *bv2 = vols->data()[i];
                    ProjFn proj = *(ProjFn *)((char *)*(void **)bv2 + 0x10);
                    Vector out;
                    proj(&out, bv2, this);
                    AEMath_Vector_assign((Vector *)&this->field_0x8, &out);
                }
            }
        }
    }
}

// ---- update_11f7e4.cpp ----
// Each child's vtable slot at +4 takes (x,y,z); update forwards its params to
// every child, then stores them as this->center.
typedef void (*UpdateFn)(void *self, float x, float y, float z);

void BoundingVolume::update(float x, float y, float z)
{
    Array<BoundingVolume *> *children = this->field_0x4;
    if (children != 0) {
        for (uint32_t i = 0; i < children->size(); i++) {
            void *child = children->data()[i];
            UpdateFn fn = *(UpdateFn *)((char *)*(void **)child + 4);
            fn(child, x, y, z);
            children = this->field_0x4;
        }
    }
    Vector v;
    v.x = x;
    v.y = y;
    v.z = z;
    AEMath_Vector_assign((Vector *)&this->field_0x8, &v);
}

// ---- getProjectionVector_11f788.cpp ----
// AbyssEngine::AEMath::operator-(Vector&out, Vector const&a, Vector const&b)
extern "C" void AEMath_operator_sub(Vector *out, const Vector *a, const Vector *b); // 0x6ec38
extern "C" void AEMath_VectorNormalize(void *out, const Vector *v);                 // 0x6ec80

// getProjectionVector(v): returns normalize(v - this->center).
Vector BoundingVolume::getProjectionVector(const Vector &v)
{
    Vector ret;
    AEMath_operator_sub(&ret, &v, (const Vector *)&this->field_0x8);
    Vector tmp;
    AEMath_VectorNormalize(&tmp, &ret);
    AEMath_Vector_assign(&ret, &tmp);
    return ret;
}
