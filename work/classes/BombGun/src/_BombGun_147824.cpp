#include "class.h"

extern "C" __attribute__((visibility("hidden"))) void *BombGun_vtable;
extern "C" void *Explosion_dtor(Explosion *self);
extern "C" void operator_delete(void *p);
extern "C" void *BombGun_base_dtor(BombGun *self);

extern "C" void *_ZN7BombGunD1Ev(BombGun *self)
{
    *(void **)self = (char *)BombGun_vtable + 8;

    Explosion *explosion = F<Explosion *>(self, 0xf0);
    if (explosion != 0)
        operator_delete(Explosion_dtor(explosion));

    F<Explosion *>(self, 0xf0) = 0;
    return BombGun_base_dtor(self);
}
