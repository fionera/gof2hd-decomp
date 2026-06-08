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
    F<float>(self, 0x58) = x;
    F<float>(self, 0x5c) = y;
    F<float>(self, 0x60) = z;
    F<int32_t>(self, 0x178) = (int32_t)x;
    F<int32_t>(self, 0x17c) = (int32_t)y;
    F<int32_t>(self, 0x180) = (int32_t)z;

    AEGeometry_setPosition(F<void *>(self, 0x8), x, y, z);
    void *m = AEGeometry_getMatrix(F<void *>(self, 0x8));
    Matrix_assign((char *)F<void *>(self, 0x4) + 0x4, m);

    char buf[12];
    AEGeometry_getPosition((Vector *)buf, F<void *>(self, 0x8));
    Vector_assign((Vector *)((char *)self + 0x2c), (Vector *)buf);

    Array<void *> *bv = F<Array<void *> *>(self, 0x128);
    if (bv != 0) {
        for (uint32_t i = 0; i < bv->length; i++) {
            void *o = bv->data[i];
            BVSetPosFn fn = *(BVSetPosFn *)(*(char **)o + 0x4);
            fn(o, F<float>(self, 0x2c), F<float>(self, 0x30), F<float>(self, 0x34));
            bv = F<Array<void *> *>(self, 0x128);
        }
    }

    if (F<void *>(self, 0x124) != 0) {
        AEGeometry_setPosition(F<void *>(self, 0x124), x, y, z);
    }
}
