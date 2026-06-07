#include "class.h"

// FModSound::pauseAll() -> for 0x8f5 slots: if (system && events[i]) Event::setPaused(events[i],1)
extern "C" int FMOD_Event_setPaused(void *event, int paused);

struct FModSound { void pauseAll(); };
void FModSound::pauseAll()
{
    int *sys = &i32(this, OFF_SYSTEM);
    void **base = (void **)((char *)this + OFF_EVENTS);
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (*sys && base[i])
            FMOD_Event_setPaused(base[i], 1);
    }
}
