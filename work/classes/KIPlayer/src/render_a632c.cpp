#include "class.h"

extern "C" void Player_render(void *player);

extern "C" void KIPlayer_render(KIPlayer *self)
{
    return Player_render(*(void **)((char *)self + 0x8));
}
