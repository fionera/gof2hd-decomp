#include "class.h"

// FModSound::stop(FMOD::Event*) -> if (e) return Event::stop(e, 0); else this
struct FModSound;
extern "C" FModSound *FMOD_Event_stop_p(void *event, int immediate);

struct FModSound { FModSound *stop(void *e); };
FModSound *FModSound::stop(void *e)
{
    if (e == 0)
        return this;
    return FMOD_Event_stop_p(e, 0);
}
