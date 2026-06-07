#include "class.h"

extern "C" void KIPlayer_setWingman(KIPlayer *self, bool b, int cmd)
{
    *(uint8_t *)((char *)self + 0xdc) = b;
    *(int *)((char *)self + 0xe0) = cmd;
    *(uint32_t *)((char *)self + 0xe4) = 1;
}
