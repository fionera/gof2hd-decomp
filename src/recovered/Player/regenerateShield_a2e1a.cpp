#include "class.h"

extern "C" void Player_regenerateShield_tail();

extern "C" void Player_regenerateShield(Player *self)
{
    float f = self->shieldHP + 1.0f;
    float maxF = (float)self->maxShieldHP;
    if (f < maxF) {
        maxF = f;
    }
    self->shieldHP = maxF;
    return Player_regenerateShield_tail();
}
