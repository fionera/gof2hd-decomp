#include "class.h"

extern "C" void *_ZN7MineGunD1Ev(MineGun *self);
extern "C" void operator_delete(void *ptr);

extern "C" void _ZN7MineGunD0Ev(MineGun *self)
{
    return operator_delete(_ZN7MineGunD1Ev(self));
}
