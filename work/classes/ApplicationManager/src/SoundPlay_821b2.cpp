#include "class.h"

extern "C" void ext_001ab4f8(void *sound, int soundId);

extern "C" void ApplicationManager_SoundPlay(ApplicationManager *self, int soundId)
{
    void *sound = *(void **)((char *)self + 0xac);
    if (sound != 0 && *(bool *)((char *)self + 0xb0)) {
        ext_001ab4f8(sound, soundId);
    }
}
