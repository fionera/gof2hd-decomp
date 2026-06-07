#include "class.h"

// FModSound::isPlaying(int) -> if (system && events[p1]) { getState(h,&s); r=(s>>3)&1; } else 0
// NOTE: target carries a stack canary for the 4-byte state slot (built with stack-protector-strong
// upstream); basic -fstack-protector here does not, so this is a near-match.
extern "C" int FMOD_Event_getState(void *event, unsigned *out);

struct FModSound { unsigned isPlaying(int p1); };
unsigned FModSound::isPlaying(int p1)
{
    if (u32(this, OFF_SYSTEM) != 0) {
        void *h = pp(this, p1 * 4 + OFF_EVENTS);
        if (h != 0) {
            unsigned s;
            FMOD_Event_getState(h, &s);
            return (s >> 3) & 1;
        }
    }
    return 0;
}
