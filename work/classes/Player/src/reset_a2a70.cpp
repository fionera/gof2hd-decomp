#include "class.h"

extern "C" void Player_updateDamageRate(Player *self);

__attribute__((minsize)) extern "C" void Player_reset(Player *self)
{
    float shield = (float)self->maxShieldHP;
    int maxHp = self->maxHitpoints;
    int maxEmp = self->maxEmpPoints;
    self->gammaHP = 100.0f;
    self->hitpoints = maxHp;
    self->empPoints = maxEmp;
    self->shieldHP = shield;
    Player_updateDamageRate(self);
    self->vulnerable = 1;
    *(unsigned short *)((char *)self + 0xc0) = 1;  // active = 1, damaged = 0
    self->field_54 = 0;
    *((unsigned char *)self + 0x44) = 0;
    self->damageDoneByPlayer = 0;
    self->field_5e = 0;
    self->field_b4 = 0;
    *((unsigned char *)self + 0x68) = 0;
    self->field_64 = 0;
    self->bombForce = 0;
    self->empForce = 0;
    *(int *)((char *)self + 0xdd) = 0;
    *(int *)((char *)self + 0xd9) = 0;
}
