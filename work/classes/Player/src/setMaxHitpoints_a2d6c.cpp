#include "class.h"

extern "C" void Player_setMaxHitpoints_tail();

extern "C" void Player_setMaxHitpoints(Player *self, int value)
{
    self->maxHitpoints = value;
    self->hitpoints = value;
    return Player_setMaxHitpoints_tail();
}
