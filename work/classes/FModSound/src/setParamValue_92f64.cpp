#include "class.h"

namespace FMOD { struct EventParameter; }

// FModSound::setParamValue(char const* name, int idx, float val)
extern "C" int FMOD_Event_getParameter(void *event, const char *name, FMOD::EventParameter **out);
extern "C" int FMOD_EventParameter_setValue(FMOD::EventParameter *p, float v);

struct FModSound { void setParamValue(const char *name, int idx, float val); };
void FModSound::setParamValue(const char *name, int idx, float val)
{
    if (idx >= 0 && u32(this, OFF_SYSTEM) != 0 && pp(this, idx * 4 + OFF_EVENTS) != 0) {
        FMOD::EventParameter *p;
        FMOD_Event_getParameter(pp(this, idx * 4 + OFF_EVENTS), name, &p);
        FMOD_EventParameter_setValue(p, val);
    }
}
