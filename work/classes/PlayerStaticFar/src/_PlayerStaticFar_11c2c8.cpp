#include "class.h"

// PlayerStaticFar deleting destructor (D0): PlayerStaticFar has no owned members,
// so it directly runs the base (PlayerStatic) destructor, then tail-calls
// operator delete (the b.w 0x1ab098 trampoline).
extern "C" void *PlayerStatic_base_dtor(PlayerStaticFar *self);
extern "C" void operator_delete(void *p);

extern "C" void _ZN15PlayerStaticFarD0Ev(PlayerStaticFar *self)
{
    return operator_delete(PlayerStatic_base_dtor(self));
}
