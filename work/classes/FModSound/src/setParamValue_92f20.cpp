#include "class.h"

namespace FMOD { struct EventParameter; }

// FModSound::setParamValue(int idx, int paramIdx, float val)
extern "C" int FMOD_Event_getParameterByIndex(void *event, int idx, FMOD::EventParameter **out);
extern "C" int FMOD_EventParameter_setValue(FMOD::EventParameter *p, float v);

struct FModSound { void setParamValue(int paramIdx, int idx, float val); };
void FModSound::setParamValue(int paramIdx, int idx, float val)
{
    if (idx >= 0 && u32(this, OFF_SYSTEM) != 0 && pp(this, idx * 4 + OFF_EVENTS) != 0) {
        FMOD::EventParameter *p;
        FMOD_Event_getParameterByIndex(pp(this, idx * 4 + OFF_EVENTS), paramIdx, &p);
        FMOD_EventParameter_setValue(p, val);
    }
}
