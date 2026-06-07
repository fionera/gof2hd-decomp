#include "class.h"

extern "C" bool KIPlayer_isDocked(KIPlayer *self)
{
    return *(int *)((char *)self + 0x88) == 9;
}
