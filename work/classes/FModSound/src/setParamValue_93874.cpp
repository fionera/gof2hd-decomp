#include "class.h"

namespace FMOD { struct Event; struct EventParameter; }

// FModSound::setParamValue(FMOD::Event* e, int paramIdx, float val)
extern "C" int FMOD_Event_getParameterByIndex(void *event, int idx, FMOD::EventParameter **out);
extern "C" int FMOD_EventParameter_setValue(FMOD::EventParameter *p, float v);

struct FModSound { void setParamValue(FMOD::Event *e, int paramIdx, float val); };
void FModSound::setParamValue(FMOD::Event *e, int paramIdx, float val)
{
    if (e != 0 && u32(this, OFF_SYSTEM) != 0) {
        FMOD::EventParameter *p;
        FMOD_Event_getParameterByIndex(e, paramIdx, &p);
        FMOD_EventParameter_setValue(p, val);
    }
}
