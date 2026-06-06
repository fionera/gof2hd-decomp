#include "class.h"

extern "C" void Player_setMaxShieldHP_tail();

extern "C" void Player_setMaxShieldHP(Player *self, int value)
{
    self->maxShieldHP = value;
    self->shieldHP = (float)value;
    return Player_setMaxShieldHP_tail();
}
