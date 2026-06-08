#include "class.h"

// FModSound::pause(int)
//   system==0 -> return this ;  h==0 -> return 0 ;  else tail-call setPaused(h,1)
extern "C" int FMOD_Event_setPaused(void *event, int paused);

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
 int pause(int p1); };
int FModSound::pause(int p1)
{
    void *self = this;
    if (u32(this, OFF_SYSTEM)) {
        self = pp(this, p1 * 4 + OFF_EVENTS);
        if (self)
            return FMOD_Event_setPaused(self, 1);
    }
    return (int)(long)self;
}
