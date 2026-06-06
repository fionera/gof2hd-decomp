#include "class.h"

extern "C" void Player_setMaxArmorHP_tail();

extern "C" void Player_setMaxArmorHP(Player *self, int value)
{
    self->armorHP = value;
    self->maxArmorHP = value;
    return Player_setMaxArmorHP_tail();
}
