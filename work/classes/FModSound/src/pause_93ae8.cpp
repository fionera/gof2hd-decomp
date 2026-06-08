#include "class.h"

// FModSound::pause(int)
//   system==0 -> return this ;  h==0 -> return 0 ;  else tail-call setPaused(h,1)
extern "C" int FMOD_Event_setPaused(void *event, int paused);

struct FModSound { int pause(int p1); };
int FModSound::pause(int p1)
{
    void *self = this;
    if (u32(this, OFF_SYSTEM)) {
        self = pp(this, p1 * 4 + OFF_EVENTS);
        if (self)
            return FMOD_Event_setPaused(self, 1);
    }
    return (int)(long)self;
}
