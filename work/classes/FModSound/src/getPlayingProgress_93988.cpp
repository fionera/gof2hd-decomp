#include "class.h"

extern "C" {
int FMOD_Event_getInfo(void *event, char **name, void *info);
float VectorSignedToFloat(int v, int mode);
}

// FModSound::getPlayingProgress(int) -> queries the FMOD event info for a slot
// and converts two of its fields to float (progress numerator/denominator).
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

    float getPlayingProgress(int idx);
};

float FModSound::getPlayingProgress(int idx)
{
    if (pp(this, OFF_FLAG2404) != 0 && pp(this, OFF_SYSTEM) != 0 &&
        u8(this, OFF_ENABLED) != 0 &&
        pp(this, idx * 4 + OFF_EVENTS) != 0) {
        char *name = 0;
        int info[8];
        FMOD_Event_getInfo(pp(this, idx * 4 + OFF_EVENTS), &name, info);
        VectorSignedToFloat(info[2], 0);
        VectorSignedToFloat(info[1], 0);
    }
    return 0.0f;
}
