#include "class.h"

extern "C" void Player_setEnemies(void *player);

extern "C" void KIPlayer_setEnemies(KIPlayer *self)
{
    return Player_setEnemies(*(void **)((char *)self + 0x4));
}
