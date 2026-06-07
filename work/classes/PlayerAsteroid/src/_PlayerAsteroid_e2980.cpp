#include "class.h"

__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_vtable;
extern "C" void *Explosion_dtor(Explosion *self);
extern "C" void *KIPlayer_dtor(PlayerAsteroid *self);

extern "C" void *_ZN14PlayerAsteroidD1Ev(PlayerAsteroid *self)
{
    F<void *>(self, 0) = (char *)PlayerAsteroid_vtable + 8;
    Explosion *explosion = F<Explosion *>(self, 0x12c);
    if (explosion != 0) {
        operator_delete(Explosion_dtor(explosion));
    }
    F<Explosion *>(self, 0x12c) = 0;
    return KIPlayer_dtor(self);
}
