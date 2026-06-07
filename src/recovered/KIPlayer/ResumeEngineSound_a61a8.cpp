#include "class.h"

extern "C" void FModSound_resumeEvent(int a, int b);

extern "C" void KIPlayer_ResumeEngineSound(KIPlayer *self)
{
    if (*(int *)((char *)self + 0x4) != 0 && *(int *)((char *)self + 0xf8) != -1) {
        return FModSound_resumeEvent(*(int *)((char *)self + 0x4), 0);
    }
}
