#include "class.h"

extern "C" void KIPlayer_setJumpSphere(KIPlayer *self, uint32_t sphere)
{
    *(uint32_t *)((char *)self + 0xd4) = sphere;
}
