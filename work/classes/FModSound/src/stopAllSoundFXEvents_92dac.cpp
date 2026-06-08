#include "class.h"

// FModSound::stopAllSoundFXEvents()
//   if (system) iterate category indices (inner skip on ==1, stop at 4): cat[i]->vtbl[0x1c](cat[i])
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
 void stopAllSoundFXEvents(); };
void FModSound::stopAllSoundFXEvents()
{
    if (u32(this, OFF_SYSTEM) == 0)
        return;
    void **cats = (void **)((char *)this + 0x23f0);
    unsigned j = 0;
    while (true) {
        unsigned i;
        do {
            i = j;
            j = i + 1;
        } while ((j & 0x7fffffff) == 1);
        if ((j & 0x7fffffff) == 4)
            break;
        void *c = cats[i];
        void **vt = *(void ***)c;
        typedef void (*Fn)(void *);
        ((Fn)vt[0x1c / 4])(c);
    }
}
