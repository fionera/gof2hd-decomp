#include "class.h"

extern "C" void Player_setGammaHP_tail();

__attribute__((minsize)) extern "C" void Player_setGammaHP(Player *self, int value)
{
    float f = (float)value;
    float sel = f;
    if (value != 9999999) {
        sel = 100.0f;
    }
    if (value > 100) {
        f = sel;
    }
    self->gammaHP = f;
    return Player_setGammaHP_tail();
}
