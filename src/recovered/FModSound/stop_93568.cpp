#include "class.h"

// FModSound::stop(int) -> if (p1>=0 && events[p1]) tail-call Event::stop(h, 0)
extern "C" int FMOD_Event_stop(void *event, int immediate);

struct FModSound { void stop(int p1); };
void FModSound::stop(int p1)
{
    if (p1 < 0)
        return;
    void *h = pp(this, p1 * 4 + OFF_EVENTS);
    if (!h)
        return;
    FMOD_Event_stop(h, 0);
}
