#include "class.h"

extern "C" __attribute__((visibility("hidden"))) void *MineGun_vtable;
extern "C" void ArrayReleaseClasses_Explosion(Array<Explosion *> *array);
extern "C" void *Array_Explosion_dtor(Array<Explosion *> *array);
extern "C" void operator_delete(void *ptr);
extern "C" void operator_delete_array(void *ptr);
extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void *ObjectGun_dtor(MineGun *self);

extern "C" void *_ZN7MineGunD1Ev(MineGun *self)
{
    P(self, 0x0) = (char *)MineGun_vtable + 8;

    Array<Explosion *> *explosions = (Array<Explosion *> *)P(self, 0xb4);
    if (explosions != 0) {
        ArrayReleaseClasses_Explosion(explosions);
        if (P(self, 0xb4) != 0) {
            operator_delete(Array_Explosion_dtor((Array<Explosion *> *)P(self, 0xb4)));
        }
        P(self, 0xb4) = 0;
    }

    operator_delete_array(P(self, 0xb8));
    AEGeometry *geometry = (AEGeometry *)P(self, 0xbc);
    P(self, 0xb8) = 0;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    P(self, 0xbc) = 0;
    return ObjectGun_dtor(self);
}
