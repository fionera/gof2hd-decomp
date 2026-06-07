#include "class.h"

extern "C" void Player_awake(void *player, int b);

extern "C" void KIPlayer_awake(KIPlayer *self)
{
    void *player = *(void **)((char *)self + 0x4);
    *(uint32_t *)((char *)self + 0x88) = 1;
    return Player_awake(player, 1);
}
