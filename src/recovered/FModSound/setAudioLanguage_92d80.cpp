#include "class.h"

// FModSound::setAudioLanguage(int)
//   if (system) tail-call ext(system, langTable[p1 == 1])
extern "C" void FMOD_setLanguage(void *system, uint32_t lang);
__attribute__((visibility("hidden"))) static const uint32_t langTable[2] = {0, 1};

struct FModSound { void setAudioLanguage(int p1); };
void FModSound::setAudioLanguage(int p1)
{
    void *system = pp(this, OFF_SYSTEM);
    if (!system)
        return;
    return FMOD_setLanguage(system, langTable[p1 == 1]);
}
