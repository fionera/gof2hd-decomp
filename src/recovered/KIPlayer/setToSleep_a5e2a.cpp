#include "class.h"

extern "C" void Player_setActive(void *player, int active);

extern "C" void KIPlayer_setToSleep(KIPlayer *self)
{
    void *player = *(void **)((char *)self + 0x4);
    *(uint32_t *)((char *)self + 0x88) = 5;
    Player_setActive(player, 0);
    *(uint8_t *)((char *)self + 0xb1) = 1;
}
