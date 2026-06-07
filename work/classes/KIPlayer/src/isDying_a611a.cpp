#include "class.h"

extern "C" bool KIPlayer_isDying(KIPlayer *self)
{
    return *(int *)((char *)self + 0x88) == 3;
}
