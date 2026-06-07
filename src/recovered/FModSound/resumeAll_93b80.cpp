#include "class.h"

// FModSound::resumeAll() -> for 0x8f5 slots: if (system && events[i]) Event::setPaused(events[i],0)
extern "C" int FMOD_Event_setPaused(void *event, int paused);

struct FModSound { void resumeAll(); };
void FModSound::resumeAll()
{
    int *sys = &i32(this, OFF_SYSTEM);
    void **base = (void **)((char *)this + OFF_EVENTS);
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (*sys && base[i])
            FMOD_Event_setPaused(base[i], 0);
    }
}
