#include "class.h"

extern "C" void FModSound_playEvent(int a, int b, int c);

extern "C" void KIPlayer_PlayEngineSound(KIPlayer *self)
{
    if (*(int *)((char *)self + 0x4) != 0 && *(int *)((char *)self + 0xf8) != -1) {
        return FModSound_playEvent(*(int *)((char *)self + 0x4),
                                   *(int *)((char *)self + 0xf8), 0);
    }
}
