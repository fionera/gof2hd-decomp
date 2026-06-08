#include "class.h"

// FModSound::getEventPauseLength(int idx)
//   if (this[0x2404] && system && enabled[0x11] && events[idx]) getProperty(h, *gProp, &out, 1)
extern "C" int FMOD_Event_getProperty(void *event, void *prop, void *out, int b);
extern "C" void **gPauseProp;   // pc-relative global: holds &prop

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
 int getEventPauseLength(int idx); };
int FModSound::getEventPauseLength(int idx)
{
    int out = 0;
    if (i32(this, OFF_FLAG2404) != 0 && u32(this, OFF_SYSTEM) != 0 && u8(this, OFF_ENABLED) != 0) {
        void *h = pp(this, idx * 4 + OFF_EVENTS);
        if (h != 0)
            FMOD_Event_getProperty(h, *gPauseProp, &out, 1);
    }
    return out;
}
