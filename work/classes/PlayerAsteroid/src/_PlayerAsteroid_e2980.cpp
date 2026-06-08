#include "class.h"

__attribute__((visibility("hidden"))) extern void *PlayerAsteroid_vtable;
extern "C" void *Explosion_dtor(Explosion *self);
extern "C" void *KIPlayer_dtor(PlayerAsteroid *self);

extern "C" void *_ZN14PlayerAsteroidD1Ev(PlayerAsteroid *self)
{
    self->f_0 = (char *)PlayerAsteroid_vtable + 8;
    Explosion *explosion = self->f_12c;
    if (explosion != 0) {
        operator_delete(Explosion_dtor(explosion));
    }
    self->f_12c = 0;
    return KIPlayer_dtor(self);
}
