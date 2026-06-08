#include "class.h"

// FModSound::enableCategory(int p1, bool enable)
extern "C" void FMOD_play(void *self, int a, void *b, float v);

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
 void enableCategory(int p1, bool enable); };
void FModSound::enableCategory(int p1, bool enable)
{
    if (p1 > 3)
        return;
    if (u32(this, OFF_SYSTEM) == 0)
        return;
    void *cat = pp(this, p1 * 4 + OFF_CATEGORY);
    if (cat == 0)
        return;

    u8(this, p1 + OFF_ENABLED) = (uint8_t)enable;
    if (!enable) {
        void **vt = *(void ***)cat;
        typedef void (*Fn)(void *);
        ((Fn)vt[0x1c / 4])(cat);
    } else if (p1 == 1 && this->f_0 >= 0) {
        FMOD_play(this, this->f_0, 0, 0.0f);
    }

    uint8_t any = 0;
    int i = 0x12;
    do {
        if ((unsigned)(i - 0x11) > 3)
            break;
        uint8_t v = u8(this, i);
        i++;
        any = (any || v) ? 1 : 0;
    } while (u8(this, i - 1) == 0);
    u8(this, OFF_ENABLED) = any;
}
