#include "class.h"

// NEAR (~82%): logic matches but the target carries a stack canary (it was built with the
// guard for the on-stack Vector). Under the fixed -fstack-protector (basic, not -strong) flag,
// clang only guards char arrays accessed as bytes; this Vector buffer (read via Vector*) is not
// protected, so no canary is emitted. Matching it would require -fstack-protector-strong.
extern "C" void AEGeometry_moveForward(void *geom, float d);
extern "C" void *AEGeometry_getMatrix(void *geom);
extern "C" void *Matrix_assign(void *dst, void *src);
extern "C" void AEGeometry_getPosition(Vector *out, void *geom);
extern "C" void *Vector_assign(Vector *dst, Vector *src);

typedef void (*BVMoveFn)(void *bv, Vector);

extern "C" void PlayerFixedObject_moveForward(PlayerFixedObject *self, int amount)
{
    float d = (float)amount;
    self->f_180 = amount + self->f_180;
    AEGeometry_moveForward(self->f_8, d);
    void *m = AEGeometry_getMatrix(self->f_8);
    Matrix_assign((char *)self->f_4 + 0x4, m);
    char buf[12];
    AEGeometry_getPosition((Vector *)buf, self->f_8);
    Vector_assign((Vector *)((char *)self + 0x2c), (Vector *)buf);
    if (self->f_124 != 0) {
        AEGeometry_moveForward(self->f_124, d);
    }
    Array<void *> *bv = F<Array<void *> *>(self, 0x128);
    if (bv != 0) {
        for (uint32_t i = 0; i < bv->length; i++) {
            void *o = bv->data[i];
            BVMoveFn fn = *(BVMoveFn *)(*(char **)o + 0x4);
            fn(o, *(Vector *)((char *)self + 0x2c));
            bv = F<Array<void *> *>(self, 0x128);
        }
    }
}
