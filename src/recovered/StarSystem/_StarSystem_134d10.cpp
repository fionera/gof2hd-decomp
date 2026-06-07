#include "class.h"

extern "C" void *Array_AEGeometry_dtor(void *array);
extern "C" void ArrayRelease_KIPlayer(void *array);
extern "C" void *Array_KIPlayer_dtor(void *array);
extern "C" void *LensFlare_dtor(void *flare);
extern "C" void ArrayRelease_uint(void *array);
extern "C" void *Array_uint_dtor(void *array);
extern "C" void ArrayRelease_int(void *array);
extern "C" void *Array_int_dtor(void *array);
extern "C" void ArrayRelease_Vector(void *array);
extern "C" void *Array_Vector_dtor(void *array);
extern "C" void operator_delete(void *ptr);

struct StarSystem {
    ~StarSystem();
};

StarSystem::~StarSystem()
{
    StarSystem *self = this;
    void *p = P(self, 0x1c);
    if (p != 0) {
        operator_delete(Array_AEGeometry_dtor(p));
    }
    P(self, 0x1c) = 0;

    p = P(self, 0x18);
    if (p != 0) {
        ArrayRelease_KIPlayer(p);
        p = P(self, 0x18);
        if (p != 0) {
            operator_delete(Array_KIPlayer_dtor(p));
        }
    }
    P(self, 0x18) = 0;

    p = P(self, 0x2c);
    if (p != 0) {
        operator_delete(LensFlare_dtor(p));
    }
    P(self, 0x2c) = 0;

    p = P(self, 0x14);
    if (p != 0) {
        ArrayRelease_uint(p);
        p = P(self, 0x14);
        if (p != 0) {
            operator_delete(Array_uint_dtor(p));
        }
    }
    P(self, 0x14) = 0;

    p = P(self, 0x24);
    if (p != 0) {
        ArrayRelease_int(p);
        p = P(self, 0x24);
        if (p != 0) {
            operator_delete(Array_int_dtor(p));
        }
    }
    P(self, 0x24) = 0;

    p = P(self, 0x20);
    if (p != 0) {
        ArrayRelease_Vector(p);
        p = P(self, 0x20);
        if (p != 0) {
            operator_delete(Array_Vector_dtor(p));
        }
    }
    P(self, 0x20) = 0;
}
