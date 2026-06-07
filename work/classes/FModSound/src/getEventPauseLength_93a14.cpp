#include "class.h"

// FModSound::getEventPauseLength(int idx)
//   if (this[0x2404] && system && enabled[0x11] && events[idx]) getProperty(h, *gProp, &out, 1)
extern "C" int FMOD_Event_getProperty(void *event, void *prop, void *out, int b);
extern "C" void **gPauseProp;   // pc-relative global: holds &prop

struct FModSound { int getEventPauseLength(int idx); };
int FModSound::getEventPauseLength(int idx)
{
    int out = 0;
    if (i32(this, OFF_FLAG2404) != 0 && u32(this, OFF_SYSTEM) != 0 && u8(this, OFF_ENABLED) != 0) {
        void *h = pp(this, idx * 4 + OFF_EVENTS);
        if (h != 0)
            FMOD_Event_getProperty(h, *gPauseProp, &out, 1);
    }
    return out;
}
