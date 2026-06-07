#include "class.h"

extern "C" void ext_001ab508(void *sound, int soundId);

extern "C" void ApplicationManager_SoundPlayLoop(ApplicationManager *self, int soundId)
{
    void *sound = *(void **)((char *)self + 0xac);
    if (sound != 0 && *(bool *)((char *)self + 0xb0)) {
        ext_001ab508(sound, soundId);
    }
}
