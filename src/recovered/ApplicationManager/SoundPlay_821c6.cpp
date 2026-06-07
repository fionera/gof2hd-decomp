#include "class.h"

extern "C" void ext_001ab488(void *sound, int soundId, float volume);

extern "C" void ApplicationManager_SoundPlay(ApplicationManager *self, int soundId, float volume)
{
    void *sound = *(void **)((char *)self + 0xac);
    if (sound == 0) {
        return;
    }
    if (*(bool *)((char *)self + 0xb0) == 0) {
        return;
    }
    ext_001ab488(sound, soundId, volume);
}
