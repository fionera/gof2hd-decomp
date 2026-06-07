#include "class.h"

extern "C" void ext_001ab568(void *sound);

extern "C" void ApplicationManager_SoundResumeSounds(ApplicationManager *self)
{
    void *sound = *(void **)((char *)self + 0xac);
    if ((sound != 0 && *(bool *)((char *)self + 0xb0)) || *(bool *)((char *)self + 0xb1)) {
        ext_001ab568(sound);
    }
}
