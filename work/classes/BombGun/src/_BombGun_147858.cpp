#include "class.h"

extern "C" void *_ZN7BombGunD1Ev(BombGun *self);
extern "C" void operator_delete(void *p);

extern "C" void _ZN7BombGunD0Ev(BombGun *self)
{
    return operator_delete(_ZN7BombGunD1Ev(self));
}
