#include "class.h"

extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void operator_delete(void *ptr);
extern "C" void ArrayReleaseClasses_AEGeometryPtr(Array *self);
extern "C" void *Array_AEGeometryPtr_dtor(Array *self);

extern "C" Explosion *_ZN9ExplosionD2Ev(Explosion *self)
{
    AEGeometry *geometry = G(self, 0x4);
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    G(self, 0x4) = 0;

    geometry = G(self, 0x8);
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    G(self, 0x8) = 0;

    Array *streaks = A(self, 0xc);
    if (streaks != 0) {
        ArrayReleaseClasses_AEGeometryPtr(streaks);
        streaks = A(self, 0xc);
        if (streaks != 0) {
            operator_delete(Array_AEGeometryPtr_dtor(streaks));
        }
    }
    A(self, 0xc) = 0;
    return self;
}
