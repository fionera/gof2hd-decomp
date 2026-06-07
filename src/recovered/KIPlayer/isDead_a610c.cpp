#include "class.h"

extern "C" bool KIPlayer_isDead(KIPlayer *self)
{
    return *(int *)((char *)self + 0x88) == 4;
}
