#include "class.h"

extern "C" BeamGun *_ZN7BeamGunD1Ev(BeamGun *self);
extern "C" void operator_delete(void *ptr);

extern "C" void _ZN7BeamGunD0Ev(BeamGun *self)
{
    return operator_delete(_ZN7BeamGunD1Ev(self));
}
