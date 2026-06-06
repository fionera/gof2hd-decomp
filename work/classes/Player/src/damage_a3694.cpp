#include "class.h"

extern "C" void Player_damage_full(Player *self, int amount, int a, int b);

extern "C" void Player_damage(Player *self, int amount)
{
    return Player_damage_full(self, amount, 0, -1);
}
