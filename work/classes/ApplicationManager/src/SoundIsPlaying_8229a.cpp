#include "class.h"

extern "C" int ext_001ab5a8(void *sound);

extern "C" int ApplicationManager_SoundIsPlaying(ApplicationManager *self)
{
    void *sound = *(void **)((char *)self + 0xac);
    if (sound == 0) {
        return 0;
    }
    return ext_001ab5a8(sound);
}
