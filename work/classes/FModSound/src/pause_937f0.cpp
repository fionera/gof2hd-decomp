#include "class.h"

// FModSound::pause(FMOD::Event*) -> int
//   if (e && system) { if (getState(e,&s)==0 && (s<<0x1c)<0) return setPaused(e,1)==0; return 0; }
extern "C" int FMOD_Event_getState(void *event, unsigned *out);
extern "C" int FMOD_Event_setPaused(void *event, int paused);

struct FModSound { int pause(void *e); };
int FModSound::pause(void *e)
{
    unsigned r = 0;
    if (e != 0 && u32(this, OFF_SYSTEM) != 0) {
        unsigned s;
        int st = FMOD_Event_getState(e, &s);
        if (st == 0 && (int)(s << 0x1c) < 0)
            r = FMOD_Event_setPaused(e, 1) == 0;
        else
            r = 0;
    }
    return r;
}
