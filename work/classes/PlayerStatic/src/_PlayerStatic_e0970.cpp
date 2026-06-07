#include "class.h"

extern "C" void *KIPlayer_dtor(PlayerStatic *self);
extern "C" void *PlayerStatic_dtor_tail();

extern "C" void *_ZN12PlayerStaticD1Ev(PlayerStatic *self)
{
    KIPlayer_dtor(self);
    return PlayerStatic_dtor_tail();
}
