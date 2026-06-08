#include "class.h"

// SpriteGun::setEnemies(Array<Player*>*) — load arg->field_8, tail-call the
// underlying Gun::setEnemies on the embedded base object.
extern "C" void SpriteGun_Gun_setEnemies(void *base);

extern "C" void SpriteGun_setEnemies(Array *param_1)
{
    return SpriteGun_Gun_setEnemies(param_1->f_8);
}
