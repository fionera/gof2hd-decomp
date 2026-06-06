#include "class.h"

extern "C" void Player_regenerateShield_tail();

extern "C" void Player_regenerateShield(Player *self, float amount)
{
    float f = self->shieldHP + amount;
    float maxF = (float)self->maxShieldHP;
    if (f < maxF) {
        maxF = f;
    }
    self->shieldHP = maxF;
    return Player_regenerateShield_tail();
}
