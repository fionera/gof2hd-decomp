#include "class.h"

extern "C" ObjectGun *_ZN9ObjectGunD1Ev(ObjectGun *self);
extern "C" void ObjectGun_delete(ObjectGun *self);

extern "C" void _ZN9ObjectGunD0Ev(ObjectGun *self)
{
    return ObjectGun_delete(_ZN9ObjectGunD1Ev(self));
}
