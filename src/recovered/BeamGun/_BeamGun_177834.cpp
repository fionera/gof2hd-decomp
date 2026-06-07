#include "class.h"

__attribute__((visibility("hidden"))) extern void *BeamGun_vtable;

extern "C" AEGeometry *AEGeometry_dtor(AEGeometry *self);
extern "C" void operator_delete(void *ptr);

extern "C" BeamGun *_ZN7BeamGunD1Ev(BeamGun *self)
{
    F<void *>(self, 0x0) = (char *)BeamGun_vtable + 8;

    AEGeometry *primary = F<AEGeometry *>(self, 0x18);
    if (primary != 0)
        operator_delete(AEGeometry_dtor(primary));
    F<AEGeometry *>(self, 0x18) = 0;

    AEGeometry *secondary = F<AEGeometry *>(self, 0x1c);
    if (secondary != 0)
        operator_delete(AEGeometry_dtor(secondary));
    F<AEGeometry *>(self, 0x1c) = 0;

    return self;
}
