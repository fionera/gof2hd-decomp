#include "class.h"

extern "C" void Player_setKIPlayer(Player *self, KIPlayer *value)
{
    self->kiPlayer = value;
}
