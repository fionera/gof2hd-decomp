#include "class.h"

extern "C" void Player_setActive(void *player);

extern "C" void KIPlayer_setActive(KIPlayer *self)
{
    return Player_setActive(*(void **)((char *)self + 0x4));
}
