#include "class.h"

extern "C" void Player_regenerateHull_tail();

extern "C" void Player_regenerateHull(Player *self)
{
    int v = self->maxHitpoints;
    if (self->hitpoints + 1 < self->maxHitpoints) {
        v = self->hitpoints + 1;
    }
    self->hitpoints = v;
    return Player_regenerateHull_tail();
}
