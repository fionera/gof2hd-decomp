#include "BoundingVolume.h"




// ---- _BoundingVolume_11f718.cpp ----
__attribute__((visibility("hidden"))) extern void *const g_BoundingVolume_vtbl;

extern "C" void ArrayReleaseClasses_BVPtr(void *arr);  // 0x757a8
extern "C" void *ArrayBV_dtor(void *arr);              // 0x757b4  Array<BoundingVolume*>::~Array
extern "C" void operator_delete(void *p);              // 0x6eb48

BoundingVolume::~BoundingVolume()
{
    pp(this, 0x0) = (void *)((char *)g_BoundingVolume_vtbl + 8);
    void *children = pp(this, 0x4);
    if (children != 0) {
        ArrayReleaseClasses_BVPtr(children);
        if (pp(this, 0x4) != 0) {
            operator_delete(ArrayBV_dtor(pp(this, 0x4)));
        }
    }
    pp(this, 0x4) = 0;
}

// ---- collide_11f870.cpp ----
// Each child is a BoundingVolume* whose vtable slot at +8 takes (x,y,z) and
// returns nonzero on collision. collide forwards this->center to every child.
// Center is reloaded as a raw 3-word block each iteration (matches ldmia).
typedef int (*CollideFn)(void *self, uint32_t x, uint32_t y, uint32_t z);

int BoundingVolume::collide(float, float, float)
{
    Array<BoundingVolume *> *children = (Array<BoundingVolume *> *)pp(this, 0x4);
    if (children != 0) {
        for (uint32_t i = 0; i < children->length; i++) {
            void *child = children->data[i];
            CollideFn fn = *(CollideFn *)((char *)*(void **)child + 8);
            uint32_t *c = (uint32_t *)((char *)this + 0x8);
            if (fn(child, c[0], c[1], c[2]) != 0) {
                return 1;
            }
            children = (Array<BoundingVolume *> *)pp(this, 0x4);
        }
    }
    return 0;
}

// ---- getCollisionNormal_11f7d8.cpp ----
// getCollisionNormal(Vector&): zero the three components of the passed vector.
void BoundingVolume::getCollisionNormal(const Vector &out)
{
    Vector *v = (Vector *)&out;
    *(uint32_t *)((char *)v + 0) = 0;
    *(uint32_t *)((char *)v + 4) = 0;
    *(uint32_t *)((char *)v + 8) = 0;
}

// ---- setVolume_11f750.cpp ----
extern "C" void *operator_new(uint32_t size);                       // 0x6eb24
extern "C" void ArrayBV_ctor(void *a);                             // 0x730c0  Array<BoundingVolume*>::Array()
// Tail-call: copy the source volume's children into the new array.
extern "C" void BoundingVolume_setVolume_tail(BoundingVolume *src, void *arr);  // 0x1ac218

void BoundingVolume::setVolume(BoundingVolume *src)
{
    void *arr = operator_new(0xc);
    ArrayBV_ctor(arr);
    this->f_4 = arr;
    return BoundingVolume_setVolume_tail(src, arr);
}

// ---- BoundingVolume_11f6dc.cpp ----
// Hidden PC-relative pointer whose target word holds the BoundingVolume vtable
// base (stored into *this, +8 to skip the RTTI header).
__attribute__((visibility("hidden"))) extern void *const g_BoundingVolume_vtbl;

BoundingVolume::BoundingVolume(float a, float b, float c, float d, float e, float f)
{
    void *vt = (void *)((char *)g_BoundingVolume_vtbl + 8);
    this->f_14 = d;
    this->f_18 = e;
    this->f_1c = f;
    this->f_0 = vt;
    this->f_4 = 0;
    this->f_8 = a;
    this->f_c = b;
    this->f_10 = c;
}

// ---- staticProjectCollisionOnSurface_11f8b0.cpp ----
extern "C" void AEMath_Vector_assign(Vector *dst, const Vector *src);  // 0x6eb3c

// child->vtable[0xc](x,y,z) -> bool collides; child->vtable[0x10](&out, child, this).
typedef int (*HitFn)(void *self, uint32_t x, uint32_t y, uint32_t z);
typedef void (*ProjFn)(Vector *out, void *self, void *bv);

// staticProjectCollisionOnSurface(v, vols): seed this->center with v, then run
// two passes projecting the point onto every colliding volume's surface.
void BoundingVolume::staticProjectCollisionOnSurface(const Vector &v, Array<BoundingVolume *> *vols)
{
    // this->center = v (copy 12 bytes).
    *(uint32_t *)((char *)this + 8) = *(const uint32_t *)((const char *)&v + 8);
    *(uint64_t *)((char *)this + 0) = *(const uint64_t *)((const char *)&v + 0);

    if (vols != 0) {
        for (int pass = 0; pass != 2; pass++) {
            for (uint32_t i = 0; i < vols->length; i++) {
                void *bv = vols->data[i];
                HitFn hit = *(HitFn *)((char *)*(void **)bv + 0xc);
                uint32_t *c = (uint32_t *)((char *)this + 0x0);
                if (hit(bv, c[0], c[1], c[2]) != 0) {
                    void *bv2 = vols->data[i];
                    ProjFn proj = *(ProjFn *)((char *)*(void **)bv2 + 0x10);
                    Vector out;
                    proj(&out, bv2, this);
                    AEMath_Vector_assign((Vector *)this, &out);
                }
            }
        }
    }
}

// ---- update_11f7e4.cpp ----
extern "C" void AEMath_Vector_assign(Vector *dst, const Vector *src);  // 0x6eb3c

// Each child's vtable slot at +4 takes (x,y,z); update forwards its params to
// every child, then stores them as this->center.
typedef void (*UpdateFn)(void *self, float x, float y, float z);

void BoundingVolume::update(float x, float y, float z)
{
    Array<BoundingVolume *> *children = (Array<BoundingVolume *> *)this->f_4;
    if (children != 0) {
        for (uint32_t i = 0; i < children->length; i++) {
            void *child = children->data[i];
            UpdateFn fn = *(UpdateFn *)((char *)*(void **)child + 4);
            fn(child, x, y, z);
            children = (Array<BoundingVolume *> *)this->f_4;
        }
    }
    Vector v;
    v.x = x;
    v.y = y;
    v.z = z;
    AEMath_Vector_assign((Vector *)((char *)this + 8), &v);
}

// ---- getProjectionVector_11f788.cpp ----
// AbyssEngine::AEMath::operator-(Vector&out, Vector const&a, Vector const&b)
extern "C" void AEMath_operator_sub(Vector *out, const Vector *a, const Vector *b); // 0x6ec38
extern "C" void AEMath_VectorNormalize(void *out, const Vector *v);                 // 0x6ec80
extern "C" void AEMath_Vector_assign(Vector *dst, const Vector *src);               // 0x6eb3c

// getProjectionVector(v): returns normalize(v - this->center).
Vector BoundingVolume::getProjectionVector(const Vector &v)
{
    Vector ret;
    AEMath_operator_sub(&ret, &v, (const Vector *)((char *)this + 8));
    char tmp[12];
    AEMath_VectorNormalize(tmp, &ret);
    AEMath_Vector_assign(&ret, (const Vector *)tmp);
    return ret;
}
