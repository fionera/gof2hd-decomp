#include "class.h"

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
