#include "class.h"

extern "C" void Player_reset(void *player);
extern "C" void KIPlayer_setToSleep(KIPlayer *self);
extern "C" void Route_reset(Route *route);

extern "C" void KIPlayer_reset(KIPlayer *self)
{
    if (*(int *)((char *)self + 0x4) != 0) {
        Player_reset(*(void **)((char *)self + 0x4));
    }
    if (*(uint8_t *)((char *)self + 0xb1) != 0 || *(uint8_t *)((char *)self + 0xb2) == 0) {
        KIPlayer_setToSleep(self);
    }
    Route *r = *(Route **)((char *)self + 0xb4);
    if (r != 0) {
        *(Route **)((char *)self + 0x6c) = r;
        Route_reset(r);
    } else if (*(Route **)((char *)self + 0x6c) != 0) {
        Route_reset(*(Route **)((char *)self + 0x6c));
    }
    *(uint32_t *)((char *)self + 0xfc) = 0;
    *(uint8_t *)((char *)self + 0x100) = 0;
}
