#include "class.h"

extern "C" void *_ZN9RocketGunD1Ev(RocketGun *self);
extern "C" void operator_delete(void *p);

extern "C" void _ZN9RocketGunD0Ev(RocketGun *self)
{
    return operator_delete(_ZN9RocketGunD1Ev(self));
}
