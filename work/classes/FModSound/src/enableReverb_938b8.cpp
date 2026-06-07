#include "class.h"

// FModSound::enableReverb(int)
//   One stack array holds both the FMOD_REVERB_PROPERTIES (buf[0..0x4f]) and numPresets (buf+0x50),
//   matching the target's frame: buffer at sp+0, scalar at sp+0x50, canary at sp+0x54.
extern "C" int FMOD_EventSystem_getNumReverbPresets(void *system, int *out);
extern "C" int FMOD_EventSystem_getReverbPresetByIndex(void *system, int idx, void *props, char **name);
extern "C" int FMOD_EventSystem_setReverbProperties(void *system, void *props);

struct FModSound { void enableReverb(int p1); };
void FModSound::enableReverb(int p1)
{
    void *system = pp(this, OFF_SYSTEM);
    if (!system)
        return;
    char buf[0x54];
    if (FMOD_EventSystem_getNumReverbPresets(system, (int *)(buf + 0x50)) != 0)
        return;
    if (*(int *)(buf + 0x50) <= p1)
        return;
    if (i32(this, 0x2408) == p1)
        return;
    i32(this, 0x2408) = p1;
    if (FMOD_EventSystem_getReverbPresetByIndex(pp(this, OFF_SYSTEM), p1, buf, 0) == 0)
        FMOD_EventSystem_setReverbProperties(pp(this, OFF_SYSTEM), buf);
}
