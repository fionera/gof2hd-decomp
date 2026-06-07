#include "class.h"

__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_resumeObj;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_resumeArg;

extern "C" int FModSound_tryToStopMusicForBGMusic();
extern "C" void ModMainMenu_resumeTail(int obj, int one, int arg);

extern "C" void _ZN11ModMainMenu8OnResumeEv(ModMainMenu *self)
{
    int *holder = g_ModMainMenu_resumeObj;
    int obj = *holder;
    if (obj == 0)
        return;
    if (FModSound_tryToStopMusicForBGMusic() != 0)
        return;
    int arg = *g_ModMainMenu_resumeArg;
    ModMainMenu_resumeTail(*holder, 1, arg);
}
