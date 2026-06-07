#include "class.h"

// Complete object destructor (D1). Sets the vtable, tears down the spark arrays and the
// base geometry, then tail-calls the base destructor.
extern "C" char PlayerGasCloud_vtable;
extern "C" void ArrayReleaseClasses_AEGeometry(void *arr);
extern "C" void *Array_AEGeometry_dtor(void *p);
extern "C" void ArrayReleaseClasses_Vector(void *arr);
extern "C" void *Array_Vector_dtor(void *p);
extern "C" void *Array_float_dtor(void *p);
extern "C" void *Array_int_dtor(void *p);
extern "C" void *AEGeometry_dtor(void *p);
extern "C" void operator_delete(void *p);
extern "C" void *PlayerGasCloud_baseDtor(void *self);

extern "C" void *_ZN14PlayerGasCloudD1Ev(void *self)
{
    *(void **)self = &PlayerGasCloud_vtable + 8;

    void *a0 = F<void *>(self, 0x138);
    if (a0 != 0) {
        ArrayReleaseClasses_AEGeometry(a0);
        void *p = F<void *>(self, 0x138);
        if (p != 0)
            operator_delete(Array_AEGeometry_dtor(p));
        F<void *>(self, 0x138) = 0;
    }

    void *a1 = F<void *>(self, 0x13c);
    if (a1 != 0) {
        ArrayReleaseClasses_Vector(a1);
        void *p = F<void *>(self, 0x13c);
        if (p != 0)
            operator_delete(Array_Vector_dtor(p));
        F<void *>(self, 0x13c) = 0;
    }

    void *a2 = F<void *>(self, 0x140);
    if (a2 != 0)
        operator_delete(Array_float_dtor(a2));
    F<void *>(self, 0x140) = 0;

    void *a3 = F<void *>(self, 0x144);
    if (a3 != 0)
        operator_delete(Array_float_dtor(a3));
    F<void *>(self, 0x144) = 0;

    void *a4 = F<void *>(self, 0x148);
    if (a4 != 0)
        operator_delete(Array_float_dtor(a4));
    F<void *>(self, 0x148) = 0;

    void *a5 = F<void *>(self, 0x14c);
    if (a5 != 0)
        operator_delete(Array_int_dtor(a5));
    F<void *>(self, 0x14c) = 0;

    void *g = F<void *>(self, 0x134);
    if (g != 0)
        operator_delete(AEGeometry_dtor(g));
    F<void *>(self, 0x134) = 0;

    return PlayerGasCloud_baseDtor(self);
}
