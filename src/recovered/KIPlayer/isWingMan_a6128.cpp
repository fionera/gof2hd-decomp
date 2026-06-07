#include "class.h"

extern "C" uint8_t KIPlayer_isWingMan(KIPlayer *self)
{
    return *(uint8_t *)((char *)self + 0xdc);
}
