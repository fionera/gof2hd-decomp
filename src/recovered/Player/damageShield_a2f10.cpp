#include "class.h"

extern "C" void Player_damageShield_tail();

__attribute__((minsize)) extern "C" void Player_damageShield(Player *self, int damage)
{
    if (!self->vulnerable) {
        return;
    }
    if (!self->active) {
        return;
    }
    float s = self->shieldHP;
    if (s <= 0.0f) {
        int h = self->hitpoints - damage;
        h &= ~(h >> 31);
        self->hitpoints = h;
    } else {
        s = s - (float)damage;
        self->shieldHP = s;
    }
    if (s < 0.0f) {
        self->shieldHP = 0;
    }
    self->damaged = 1;
    return Player_damageShield_tail();
}
