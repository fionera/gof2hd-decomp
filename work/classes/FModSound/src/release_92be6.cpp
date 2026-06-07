#include "class.h"

// FModSound::release()
extern "C" void FMOD_EventSystem_unload(void *system);
extern "C" void FMOD_EventSystem_release(void *system);

struct FModSound { void release(); };
void FModSound::release()
{
    void *system = pp(this, OFF_SYSTEM);
    if (system != 0) {
        FMOD_EventSystem_unload(system);
        FMOD_EventSystem_release(pp(this, OFF_SYSTEM));
        u32(this, OFF_SYSTEM) = 0;
    }
    uint32_t *p = (uint32_t *)((char *)this + OFF_EVENTS);
    for (unsigned i = 0; i < 0x8f5u; ++i)
        *p++ = 0;
    for (int i = 0; i != 4; i++)
        u32(this, i * 4 + OFF_CATEGORY) = 0;
}
