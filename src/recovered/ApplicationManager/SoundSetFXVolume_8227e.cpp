#include "class.h"

extern "C" void ext_001ab588(void *sound);

extern "C" void ApplicationManager_SoundSetFXVolume(ApplicationManager *self, int volume)
{
    void *sound = *(void **)((char *)self + 0xac);
    if (sound != 0) {
        ext_001ab588(sound);
    }
}
