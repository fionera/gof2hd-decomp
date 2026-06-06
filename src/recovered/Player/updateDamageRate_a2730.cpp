#include "class.h"

extern "C" void Player_updateDamageRate(Player *self)
{
    float maxHp = (float)self->maxHitpoints;
    float hp = (float)self->hitpoints;
    float maxArmor = (float)self->maxArmorHP;
    float armor = (float)self->armorHP;
    float maxEmp = (float)self->maxEmpPoints;
    float maxShield = (float)self->maxShieldHP;
    float emp = (float)self->empPoints;

    float shieldRate = (self->shieldHP / maxShield) * 100.0f;
    float armorRate = (armor / maxArmor) * 100.0f;
    float empRate = (emp / maxEmp) * 100.0f;

    self->damageRate = (int)((hp / maxHp) * 100.0f);
    self->shieldDamageRate = (int)shieldRate;
    self->armorDamageRate = (int)armorRate;
    self->empDamageRate = (int)empRate;
}
