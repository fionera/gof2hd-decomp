#include "class.h"

extern "C" void KIPlayer_setWingmanCommand(KIPlayer *self, int cmd, KIPlayer *target)
{
    *(int *)((char *)self + 0xe4) = cmd;
    *(KIPlayer **)((char *)self + 0xe8) = target;
}
