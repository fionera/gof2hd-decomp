#include "class.h"

extern "C" void Player_damageHull_tail();

__attribute__((minsize)) extern "C" void Player_damageHull(Player *self, int damage)
{
    if (!self->vulnerable) {
        return;
    }
    if (!self->active) {
        return;
    }
    int armor = self->armorHP;
    if (armor <= 0) {
        int h = self->hitpoints - damage;
        h &= ~(h >> 31);
        self->hitpoints = h;
    } else {
        armor -= damage;
        self->armorHP = armor;
    }
    if (armor <= -1) {
        self->armorHP = 0;
    }
    self->damaged = 1;
    return Player_damageHull_tail();
}
