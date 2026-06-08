#include "class.h"

__attribute__((visibility("hidden"))) extern "C" void *ObjectGun_vtable;

extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void operator_delete(void *self);
extern "C" void ArrayReleaseClasses_Explosion(Array *self);
extern "C" void *Array_Explosion_dtor(Array *self);
extern "C" void operator_delete_array(void *self);

extern "C" ObjectGun *_ZN9ObjectGunD1Ev(ObjectGun *self)
{
    self->f_0 = (char *)ObjectGun_vtable + 8;

    AEGeometry *geometry = self->f_18;
    if (geometry != 0)
        operator_delete(AEGeometry_dtor(geometry));

    Array *explosions = self->f_2c;
    self->f_18 = 0;
    if (explosions != 0) {
        ArrayReleaseClasses_Explosion(explosions);
        explosions = self->f_2c;
        if (explosions != 0)
            operator_delete(Array_Explosion_dtor(explosions));
        self->f_2c = 0;
    }

    operator_delete_array(self->f_30);
    self->f_30 = 0;
    return self;
}
