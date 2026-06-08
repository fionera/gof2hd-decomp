#include "class.h"

extern "C" void AEGeometry_setPosition(void *geom, float x, float y, float z);
extern "C" void *AEGeometry_getMatrix(void *geom);
extern "C" void *Matrix_assign(void *dst, void *src);
extern "C" void AEGeometry_getPosition(Vector *out, void *geom);
extern "C" void *Vector_assign(Vector *dst, Vector *src);

typedef void (*BVSetPosFn)(void *bv, float, float, float);

// PlayerFixedObject::setPosition(float, float, float)
extern "C" void PlayerFixedObject_setPosition3(PlayerFixedObject *self, float x, float y, float z)
{
    self->f_58 = x;
    self->f_5c = y;
    self->f_60 = z;
    self->f_178 = (int32_t)x;
    self->f_17c = (int32_t)y;
    self->f_180 = (int32_t)z;

    AEGeometry_setPosition(self->f_8, x, y, z);
    void *m = AEGeometry_getMatrix(self->f_8);
    Matrix_assign((char *)self->f_4 + 0x4, m);

    char buf[12];
    AEGeometry_getPosition((Vector *)buf, self->f_8);
    Vector_assign((Vector *)((char *)self + 0x2c), (Vector *)buf);

    Array<void *> *bv = F<Array<void *> *>(self, 0x128);
    if (bv != 0) {
        for (uint32_t i = 0; i < bv->length; i++) {
            void *o = bv->data[i];
            BVSetPosFn fn = *(BVSetPosFn *)(*(char **)o + 0x4);
            fn(o, self->f_2c, self->f_30, self->f_34);
            bv = F<Array<void *> *>(self, 0x128);
        }
    }

    if (self->f_124 != 0) {
        AEGeometry_setPosition(self->f_124, x, y, z);
    }
}
