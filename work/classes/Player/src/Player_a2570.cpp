#include "class.h"

void *operator new(uint32_t size);

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

extern "C" void Matrix_ctor(Matrix *self);
extern "C" void Player_updateDamageRate(Player *self);
extern "C" void Array_GunArrayArray_ctor(Array<Array<Gun *> *> *array);
extern "C" void Array_Gun_ctor(Array<Gun *> *array);
extern "C" void ArraySetLength_GunArrayArray(int len, Array<Array<Gun *> *> *array);
extern "C" void ArraySetLength_Gun(int len, Array<Gun *> *array);
extern "C" void MatrixGetPosition(void *out, float *matrix);
extern "C" void Vector_assign(Vector *dst, Vector *src);

extern "C" Player *Player_ctor(Player *self, int radius, int hitpoints,
                               int numPrimary, int numSecondary, int numTertiary)
{
    void * volatile cookie = __stack_chk_guard;
    Matrix_ctor((Matrix *)self->transform);
    self->shieldHP = 0.0f;
    self->armorHP = 0;
    self->maxArmorHP = 0;
    self->maxShieldHP = 0;
    self->hitVector[0] = 0.0f;
    self->hitVector[1] = 0.0f;
    self->hitVector[2] = 0.0f;
    self->position[1] = 0.0f;
    self->position[2] = 0.0f;
    self->radius = radius;
    self->numPrimaryGuns = numPrimary;
    self->numSecondaryGuns = numSecondary;
    self->numTertiaryGuns = numTertiary;
    self->gammaHP = 100.0f;
    self->hitpoints = hitpoints;
    self->empPoints = 0;
    self->maxEmpPoints = 0;
    self->maxHitpoints = hitpoints;
    self->damageDoneByPlayer = 0;
    self->field_5e = 0;
    self->shootingEnabled = 1;
    self->position[0] = 0.0f;
    self->field_60 = 0;
    self->bombForce = 0.0f;
    self->empForce = 0.0f;
    Player_updateDamageRate(self);
    self->field_58 = -1;

    Array<Array<Gun *> *> *gunArr =
        static_cast<Array<Array<Gun *> *> *>(operator new(sizeof(Array<Array<Gun *> *>)));
    Array_GunArrayArray_ctor(gunArr);
    self->guns = gunArr;
    ArraySetLength_GunArrayArray(3, gunArr);

    if (numPrimary < 1) {
        self->guns->data[0] = 0;
    } else {
        Array<Gun *> *a = static_cast<Array<Gun *> *>(operator new(sizeof(Array<Gun *>)));
        Array_Gun_ctor(a);
        self->guns->data[0] = a;
        ArraySetLength_Gun(numPrimary, self->guns->data[0]);
    }
    if (numSecondary < 1) {
        self->guns->data[1] = 0;
    } else {
        Array<Gun *> *a = static_cast<Array<Gun *> *>(operator new(sizeof(Array<Gun *>)));
        Array_Gun_ctor(a);
        self->guns->data[1] = a;
        ArraySetLength_Gun(numSecondary, self->guns->data[1]);
    }
    if (numTertiary < 1) {
        self->guns->data[2] = 0;
    } else {
        Array<Gun *> *a = static_cast<Array<Gun *> *>(operator new(sizeof(Array<Gun *>)));
        Array_Gun_ctor(a);
        self->guns->data[2] = a;
        ArraySetLength_Gun(numTertiary, self->guns->data[2]);
    }

    self->playShootSound = 1;
    self->playShootSoundId = 1;
    self->field_44 = 0;
    self->active = 1;
    self->field_b4 = 0;
    self->vulnerable = 1;
    self->kiPlayer = 0;
    self->bombForce = 0.0f;
    self->enemyFlags = 0;
    self->turnedEnemy = 0;
    *(uint16_t *)&self->alwaysEnemy = 0;
    self->enemies = 0;
    self->field_54 = 0;
    self->field_68 = 0;
    self->neverAttack = 0;
    self->field_110 = 0;
    self->engineEvent = 0;
    self->field_108 = 0;
    self->field_f8 = 0;
    self->field_64 = 0;

    float tmp[3];
    MatrixGetPosition(tmp, self->transform);
    Vector_assign((Vector *)self->position, (Vector *)tmp);
    self->field_f4 = -1;

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta == 0) {
        return self;
    }
    __stack_chk_fail(guardDelta);
}
