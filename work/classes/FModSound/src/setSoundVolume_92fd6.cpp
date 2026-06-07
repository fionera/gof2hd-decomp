#include "class.h"

// FModSound::setSoundVolume(int, float) -> if (system && events[p1]) tail-call setVolume(h, vol)
extern "C" int FMOD_Event_setVolume(void *event, float vol);

struct FModSound { void setSoundVolume(int p1, float vol); };
void FModSound::setSoundVolume(int p1, float vol)
{
    if (u32(this, OFF_SYSTEM) != 0) {
        void *h = pp(this, p1 * 4 + OFF_EVENTS);
        if (h)
            FMOD_Event_setVolume(h, vol);
    }
}
