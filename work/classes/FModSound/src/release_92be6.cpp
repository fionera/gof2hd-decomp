#include "class.h"

// FModSound::release()
extern "C" void FMOD_EventSystem_unload(void *system);
extern "C" void FMOD_EventSystem_release(void *system);

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
 void release(); };
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
