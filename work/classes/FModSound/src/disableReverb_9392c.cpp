#include "class.h"

// FModSound::disableReverb()
//   if (system) { memcpy8(buf, &kRev, 0x50); setReverbProperties(system, buf); }
//   this->0x2408 = -1;
extern "C" void *__aeabi_memcpy8(void *dst, const void *src, uint32_t n);
extern "C" int FMOD_EventSystem_setReverbProperties(void *system, void *props);

__attribute__((visibility("hidden"))) static const unsigned char kRev[0x50] = {0};

struct FModSound { void disableReverb(); };
void FModSound::disableReverb()
{
    void *system = pp(this, OFF_SYSTEM);
    if (system) {
        unsigned char buf[0x50];
        __aeabi_memcpy8(buf, kRev, 0x50);
        FMOD_EventSystem_setReverbProperties(system, buf);
    }
    i32(this, 0x2408) = -1;
}
