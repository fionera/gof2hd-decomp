#include "class.h"

// PlayerStaticFar base-object destructor (D2): no owned members, so the whole
// body is a single tail-call thunk forwarding to the base PlayerStatic dtor
// (b.w into the trampoline).
extern "C" void *PlayerStatic_base_dtor_thunk(PlayerStaticFar *self);

extern "C" void *_ZN15PlayerStaticFarD2Ev(PlayerStaticFar *self)
{
    return PlayerStatic_base_dtor_thunk(self);
}
