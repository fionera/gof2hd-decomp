#include "class.h"

extern "C" void *PlayerStaticFar_dtor(void *self);
extern "C" void operator_delete(void *self);

extern "C" void _ZN14PlayerWormHoleD0Ev(void *self)
{
    return operator_delete(PlayerStaticFar_dtor(self));
}
