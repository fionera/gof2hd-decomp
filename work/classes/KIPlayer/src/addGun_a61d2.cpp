#include "class.h"

extern "C" void Player_addGun_a(void *player);

extern "C" void KIPlayer_addGun_a(KIPlayer *self)
{
    return Player_addGun_a(*(void **)((char *)self + 0x4));
}
