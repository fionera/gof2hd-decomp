#include "class.h"

// FModSound::setVolume(int, float)
//   if (system && (h=this[p1*4+0x23ec])) tail-call h->vtbl[0x20](h, vol)
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
 void setVolume(int p1, float vol); };
void FModSound::setVolume(int p1, float vol)
{
    if (u32(this, OFF_SYSTEM) == 0)
        return;
    void *h = pp(this, p1 * 4 + OFF_CATEGORY);
    if (!h)
        return;
    void **vt = *(void ***)h;
    typedef void (*Fn)(void *, float);
    return ((Fn)vt[0x20 / 4])(h, vol);
}
