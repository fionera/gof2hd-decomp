#include "class.h"

__attribute__((visibility("hidden"))) extern "C" void *ObjectGun_vtable;

extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void operator_delete(void *self);
extern "C" void ArrayReleaseClasses_Explosion(Array *self);
extern "C" void *Array_Explosion_dtor(Array *self);
extern "C" void operator_delete_array(void *self);

extern "C" ObjectGun *_ZN9ObjectGunD1Ev(ObjectGun *self)
{
    F<void *>(self, 0x0) = (char *)ObjectGun_vtable + 8;

    AEGeometry *geometry = F<AEGeometry *>(self, 0x18);
    if (geometry != 0)
        operator_delete(AEGeometry_dtor(geometry));

    Array *explosions = F<Array *>(self, 0x2c);
    F<AEGeometry *>(self, 0x18) = 0;
    if (explosions != 0) {
        ArrayReleaseClasses_Explosion(explosions);
        explosions = F<Array *>(self, 0x2c);
        if (explosions != 0)
            operator_delete(Array_Explosion_dtor(explosions));
        F<Array *>(self, 0x2c) = 0;
    }

    operator_delete_array(F<void *>(self, 0x30));
    F<void *>(self, 0x30) = 0;
    return self;
}
