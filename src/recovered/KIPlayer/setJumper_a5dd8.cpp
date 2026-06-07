#include "class.h"

extern "C" void KIPlayer_setJumper(KIPlayer *self, bool b)
{
    *(uint8_t *)((char *)self + 0xec) = b;
}
