#include "class.h"

// Two singleton holders (single pc-rel deref each).
__attribute__((visibility("hidden"))) extern void **g_ModStation_resumeObj;
__attribute__((visibility("hidden"))) extern void **g_ModStation_resumeArg;
extern "C" int FModSound_tryToStopMusicForBGMusic();
// Tail veneer (0x1ac178): (obj, 1, arg)
extern "C" void ModStation_resumeTail(void *obj, int one, int arg);

// ModStation::OnResume()
extern "C" void ModStation_OnResume()
{
    void **holder = g_ModStation_resumeObj;
    void *obj = *holder;
    if (obj == 0)
        return;
    if (FModSound_tryToStopMusicForBGMusic() != 0)
        return;
    int arg = *(int *)*g_ModStation_resumeArg;
    ModStation_resumeTail(*holder, 1, arg);
}
