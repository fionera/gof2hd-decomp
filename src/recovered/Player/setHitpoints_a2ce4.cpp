#include "class.h"

extern "C" void Player_setHitpoints_tail();

extern "C" void Player_setHitpoints(Player *self, int value)
{
    self->hitpoints = value;
    if (self->maxHitpoints < value) {
        self->maxHitpoints = value;
    }
    return Player_setHitpoints_tail();
}
