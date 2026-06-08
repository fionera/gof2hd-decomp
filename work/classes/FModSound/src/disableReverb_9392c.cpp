#include "class.h"

// FModSound::disableReverb()
//   if (system) { memcpy8(buf, &kRev, 0x50); setReverbProperties(system, buf); }
//   this->0x2408 = -1;
extern "C" void *__aeabi_memcpy8(void *dst, const void *src, uint32_t n);
extern "C" int FMOD_EventSystem_setReverbProperties(void *system, void *props);

__attribute__((visibility("hidden"))) static const unsigned char kRev[0x50] = {0};

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
 void disableReverb(); };
void FModSound::disableReverb()
{
    void *system = pp(this, OFF_SYSTEM);
    if (system) {
        unsigned char buf[0x50];
        __aeabi_memcpy8(buf, kRev, 0x50);
        FMOD_EventSystem_setReverbProperties(system, buf);
    }
    this->f_2408 = -1;
}
