#include "class.h"

extern "C" void Player_setShieldHP_tail();

__attribute__((minsize)) extern "C" void Player_setShieldHP(Player *self, int value)
{
    float maxF = (float)self->maxShieldHP;
    self->shieldHP = (float)value;
    if ((float)value > maxF) {
        self->shieldHP = maxF;
    }
    return Player_setShieldHP_tail();
}
