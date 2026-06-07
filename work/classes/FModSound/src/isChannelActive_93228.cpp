#include "class.h"

// FModSound::isChannelActive(int) -> if (system && events[p1]) { getState(h,&s); r=(s>>4)&1; } else 0
extern "C" int FMOD_Event_getState(void *event, unsigned *out);

struct FModSound { unsigned isChannelActive(int p1); };
unsigned FModSound::isChannelActive(int p1)
{
    if (u32(this, OFF_SYSTEM) != 0) {
        void *h = pp(this, p1 * 4 + OFF_EVENTS);
        if (h != 0) {
            unsigned s;
            FMOD_Event_getState(h, &s);
            return (s >> 4) & 1;
        }
    }
    return 0;
}
