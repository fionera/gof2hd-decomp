#include "class.h"

extern "C" void KIPlayer_jump(KIPlayer *self)
{
    *(uint8_t *)((char *)self + 0xf4) = 0;
    *(uint8_t *)((char *)self + 0xec) = 1;
}
