#include "class.h"

extern "C" void Player_damageShip(Player *self, int damage)
{
    int v = self->hitpoints - damage;
    v &= ~(v >> 31);
    self->hitpoints = v;
}
