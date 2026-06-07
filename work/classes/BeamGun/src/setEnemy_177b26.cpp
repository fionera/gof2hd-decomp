#include "class.h"

extern "C" void BeamGun_setEnemy_tail(void *data);

extern "C" void BeamGun_setEnemy(Player *enemy)
{
    return BeamGun_setEnemy_tail(F<void *>(enemy, 0x8));
}
