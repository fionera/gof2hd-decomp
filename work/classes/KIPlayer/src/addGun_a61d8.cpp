#include "class.h"

extern "C" void Player_addGun_b(void *player);

extern "C" void KIPlayer_addGun_b(KIPlayer *self)
{
    return Player_addGun_b(*(void **)((char *)self + 0x4));
}
