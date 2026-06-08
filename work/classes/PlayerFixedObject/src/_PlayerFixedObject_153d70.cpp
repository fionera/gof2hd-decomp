#include "class.h"

// Complete object destructor (D1). Returns this. Tears down the wrecked-mesh geometry,
// the two bounding-volume arrays, the explosion, and the name String, then tail-calls
// the base destructor.
extern "C" char PlayerFixedObject_vtable;
extern "C" void *AEGeometry_dtor(void *p);
extern "C" void ArrayReleaseClasses_BV(void *arr);
extern "C" void *Array_BV_dtor(void *p);
extern "C" void *Explosion_dtor(void *p);
extern "C" void String_dtor(void *p);
extern "C" void operator_delete(void *p);
extern "C" void *PlayerFixedObject_baseDtor(PlayerFixedObject *self);

extern "C" void *_ZN17PlayerFixedObjectD1Ev(PlayerFixedObject *self)
{
    void *wreck = self->f_124;
    *(void **)self = &PlayerFixedObject_vtable + 8;
    if (wreck != self->f_8) {
        if (wreck != 0) operator_delete(AEGeometry_dtor(wreck));
        self->f_124 = 0;
    }
    void *bvB = self->f_128;
    if (bvB != 0) {
        ArrayReleaseClasses_BV(bvB);
        void *b = self->f_128;
        if (b != 0) operator_delete(Array_BV_dtor(b));
    }
    self->f_128 = 0;
    void *bvA = self->f_12c;
    if (bvA != 0) {
        ArrayReleaseClasses_BV(bvA);
        void *a = self->f_12c;
        if (a != 0) operator_delete(Array_BV_dtor(a));
    }
    self->f_12c = 0;
    void *expl = self->f_18c;
    if (expl != 0) operator_delete(Explosion_dtor(expl));
    self->f_18c = 0;
    String_dtor((char *)self + 0x1ac);
    return PlayerFixedObject_baseDtor(self);
}
