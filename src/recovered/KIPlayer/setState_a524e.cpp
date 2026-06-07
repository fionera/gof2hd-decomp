#include "class.h"

extern "C" void KIPlayer_setState(KIPlayer *self, int state)
{
    *(int *)((char *)self + 0x88) = state;
}
