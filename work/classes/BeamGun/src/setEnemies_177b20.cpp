#include "class.h"

extern "C" void BeamGun_setEnemies_tail(void *data);

extern "C" void BeamGun_setEnemies(Array *enemies)
{
    return BeamGun_setEnemies_tail(F<void *>(enemies, 0x8));
}
