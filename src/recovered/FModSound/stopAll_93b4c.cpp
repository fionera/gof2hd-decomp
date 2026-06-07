#include "class.h"

// FModSound::stopAll() -> for 0x8f5 slots: if (system && events[i]) Event::stop(events[i],1)
extern "C" int FMOD_Event_stop(void *event, int immediate);

struct FModSound { void stopAll(); };
void FModSound::stopAll()
{
    int *sys = &i32(this, OFF_SYSTEM);
    void **base = (void **)((char *)this + OFF_EVENTS);
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (*sys && base[i])
            FMOD_Event_stop(base[i], 1);
    }
}
