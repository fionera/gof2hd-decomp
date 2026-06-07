#include "class.h"

extern "C" void ext_001ab528(void *sound);

extern "C" void ApplicationManager_SoundStopSounds(ApplicationManager *self)
{
    void *sound = *(void **)((char *)self + 0xac);
    if (sound != 0) {
        ext_001ab528(sound);
    }
}
