#include "class.h"

extern "C" void KIPlayer_setActive(KIPlayer *self);

extern "C" void KIPlayer_setInitActive(KIPlayer *self)
{
    KIPlayer_setActive(self);
    *(uint8_t *)((char *)self + 0xb2) = 0;
}
