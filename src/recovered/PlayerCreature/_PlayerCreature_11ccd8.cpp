#include "class.h"

extern "C" PlayerCreature *KIPlayer_dtor(KIPlayer *self);
extern "C" void PlayerCreature_dtor_tail(PlayerCreature *self);

extern "C" void _ZN14PlayerCreatureD1Ev(PlayerCreature *self)
{
    PlayerCreature_dtor_tail(KIPlayer_dtor((KIPlayer *)self));
}
