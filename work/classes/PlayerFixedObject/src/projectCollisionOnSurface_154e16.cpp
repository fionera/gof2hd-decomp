#include "class.h"

// Returns a Vector by value (sret r0, this r1, collision vector r2). The callee returns
// the projected Vector into the same sret, so the compiler keeps a frame + call (not tail).
struct V3 { float x, y, z; };
extern "C" V3 BV_staticProjectCollisionOnSurface(void *vec, void *bvArray);

extern "C" V3 PlayerFixedObject_projectCollisionOnSurface(PlayerFixedObject *self, void *vec)
{
    void *bv = self->f_12c;
    if (bv != 0 && self->f_88 == 4) {
        return BV_staticProjectCollisionOnSurface(vec, bv);
    }
    void *bv2 = self->f_128;
    if (bv2 != 0) {
        return BV_staticProjectCollisionOnSurface(vec, bv2);
    }
    V3 z = {0.0f, 0.0f, 0.0f};
    return z;
}
