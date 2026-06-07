#include "class.h"

// FModSound::resume(FMOD::Event*) -> bool: e && system && setPaused(e,0)==0
extern "C" int FMOD_Event_setPaused(void *event, int paused);

struct FModSound { bool resume(void *e); };
bool FModSound::resume(void *e)
{
    bool ok = false;
    if (e != 0 && u32(this, OFF_SYSTEM) != 0) {
        int r = FMOD_Event_setPaused(e, 0);
        ok = r == 0;
    }
    return ok;
}
