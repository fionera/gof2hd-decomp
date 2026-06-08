#include "class.h"

extern "C" void ObjectGun_setEnemies_impl(void *items);

void ObjectGun::setEnemies(Array *enemies)
{
    return ObjectGun_setEnemies_impl(enemies->f_8);
}
