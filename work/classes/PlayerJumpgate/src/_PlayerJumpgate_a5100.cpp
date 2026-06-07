#include "class.h"

extern "C" void PlayerStaticFar_dtor(PlayerStaticFar *self);
extern "C" void *PlayerJumpgate_delete_tail();

extern "C" void *_ZN14PlayerJumpgateD2Ev(PlayerJumpgate *self)
{
    PlayerStaticFar_dtor((PlayerStaticFar *)self);
    return PlayerJumpgate_delete_tail();
}
