#include "class.h"

extern "C" void ext_001ab578(void *sound, int volume);

extern "C" void ApplicationManager_SoundSetVolume(ApplicationManager *self, int volume)
{
    void *sound = *(void **)((char *)self + 0xac);
    if (sound != 0) {
        ext_001ab578(sound, volume);
    }
}
