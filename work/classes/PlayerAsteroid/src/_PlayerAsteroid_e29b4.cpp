#include "class.h"

extern "C" void *PlayerAsteroid_complete_dtor(PlayerAsteroid *self);

extern "C" void _ZN14PlayerAsteroidD0Ev(PlayerAsteroid *self)
{
    return operator_delete(PlayerAsteroid_complete_dtor(self));
}
