#include "class.h"

extern "C" void Player_regenerateArmor_tail();

extern "C" void Player_regenerateArmor(Player *self)
{
    int v = self->armorHP + 2;
    if (v > self->maxArmorHP) {
        v = self->maxArmorHP;
    }
    self->armorHP = v;
    return Player_regenerateArmor_tail();
}
