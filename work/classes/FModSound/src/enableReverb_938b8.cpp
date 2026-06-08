#include "class.h"

// FModSound::enableReverb(int)
//   One stack array holds both the FMOD_REVERB_PROPERTIES (buf[0..0x4f]) and numPresets (buf+0x50),
//   matching the target's frame: buffer at sp+0, scalar at sp+0x50, canary at sp+0x54.
extern "C" int FMOD_EventSystem_getNumReverbPresets(void *system, int *out);
extern "C" int FMOD_EventSystem_getReverbPresetByIndex(void *system, int idx, void *props, char **name);
extern "C" int FMOD_EventSystem_setReverbProperties(void *system, void *props);

struct FModSound {
    // @portable-fields
    int f_0; // 0x0
    int f_4; // 0x4
    uint8_t f_8; // 0x8
    unsigned char _pad_9[3];
    void* f_c; // 0xc
    uint8_t f_10; // 0x10
    unsigned char _pad_11[9199];
    int f_2400; // 0x2400
    int f_2404; // 0x2404
    int f_2408; // 0x2408
    int f_240c; // 0x240c
    unsigned char _pad_2410[20];
    void* f_2424; // 0x2424
    void* f_2428; // 0x2428
    void* f_242c; // 0x242c
    void* f_2430; // 0x2430
    void* f_2434; // 0x2434
    void* f_2438; // 0x2438
 void enableReverb(int p1); };
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
    if (this->f_2408 == p1)
        return;
    this->f_2408 = p1;
    if (FMOD_EventSystem_getReverbPresetByIndex(pp(this, OFF_SYSTEM), p1, buf, 0) == 0)
        FMOD_EventSystem_setReverbProperties(pp(this, OFF_SYSTEM), buf);
}
