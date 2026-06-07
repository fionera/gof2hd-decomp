#include "class.h"

extern "C" void Player_setDead(void *self, int arg);

extern "C" void KIPlayer_setDead(KIPlayer *self)
{
    *(uint32_t *)((char *)self + 0x88) = 4;
    return Player_setDead(self, 0);
}
