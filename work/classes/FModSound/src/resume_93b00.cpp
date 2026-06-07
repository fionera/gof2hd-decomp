#include "class.h"

// FModSound::resume(int) -> if (system && events[p1]) tail-call setPaused(h, 0)
extern "C" int FMOD_Event_setPaused(void *event, int paused);

struct FModSound { void resume(int p1); };
void FModSound::resume(int p1)
{
    if (u32(this, OFF_SYSTEM) != 0) {
        void *h = pp(this, p1 * 4 + OFF_EVENTS);
        if (h)
            FMOD_Event_setPaused(h, 0);
    }
}
