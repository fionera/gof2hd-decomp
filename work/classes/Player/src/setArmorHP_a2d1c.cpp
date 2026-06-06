#include "class.h"

extern "C" void Player_setArmorHP_tail();

extern "C" void Player_setArmorHP(Player *self, int value)
{
    if (self->maxArmorHP < value) {
        value = self->maxArmorHP;
    }
    self->armorHP = value;
    return Player_setArmorHP_tail();
}
