#include "class.h"

extern "C" uint8_t KIPlayer_isEnemy(KIPlayer *self)
{
    return *(uint8_t *)(*(char **)((char *)self + 0x4) + 0x5c);
}
