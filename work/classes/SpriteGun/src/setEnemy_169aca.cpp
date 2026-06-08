#include "class.h"

// SpriteGun::setEnemy(Player*) — load arg->field_8, tail-call Gun::setEnemy on base.
extern "C" void SpriteGun_Gun_setEnemy(void *base);

extern "C" void SpriteGun_setEnemy(Player *param_1)
{
    return SpriteGun_Gun_setEnemy(param_1->f_8);
}
