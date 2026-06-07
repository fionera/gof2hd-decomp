#include "class.h"

namespace FMOD { struct EventParameter; }

// FModSound::getParam(char const*, int) -> if (system && events[idx]) getParameter(h, name, &out)
extern "C" int FMOD_Event_getParameter(void *event, const char *name, FMOD::EventParameter **out);

struct FModSound { void getParam(const char *name, int idx); };
void FModSound::getParam(const char *name, int idx)
{
    if (u32(this, OFF_SYSTEM) != 0) {
        void *h = pp(this, idx * 4 + OFF_EVENTS);
        if (h != 0) {
            FMOD::EventParameter *out;
            FMOD_Event_getParameter(h, name, &out);
        }
    }
}
