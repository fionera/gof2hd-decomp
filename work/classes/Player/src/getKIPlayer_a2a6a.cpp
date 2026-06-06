#include "class.h"

extern "C" KIPlayer *Player_getKIPlayer(Player *self)
{
    return self->kiPlayer;
}
