#include "class.h"

extern "C" void ext_001ab558(void *sound, int soundId);

extern "C" void ApplicationManager_SoundResume(ApplicationManager *self, int soundId)
{
    void *sound = *(void **)((char *)self + 0xac);
    if ((sound != 0 && *(bool *)((char *)self + 0xb0)) || *(bool *)((char *)self + 0xb1)) {
        ext_001ab558(sound, soundId);
    }
}
