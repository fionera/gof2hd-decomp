#include "class.h"

extern "C" void FModSound_stopEvent(int player);

extern "C" void KIPlayer_StopEngineSound(KIPlayer *self)
{
    int player = *(int *)((char *)self + 0x4);
    if (player != 0 && *(int *)((char *)self + 0xf8) != -1) {
        return FModSound_stopEvent(player);
    }
}
