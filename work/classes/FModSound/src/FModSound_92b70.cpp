#include "class.h"

// FModSound::FModSound()
extern "C" void *AEFile_GetAppRootDir();

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
 FModSound(); };
FModSound::FModSound()
{
    u32(this, OFF_SYSTEM) = 0;
    this->f_2400 = 0;
    this->f_2404 = 0;
    this->f_c = AEFile_GetAppRootDir();
    this->f_10 = 1;
    for (int i = 0x11; i != 0x15; i++)
        u8(this, i) = 1;
    for (int i = 0; i != 0x23d4; i += 4)
        u32(this, i + OFF_EVENTS) = 0;
    this->f_2438 = 0;
    this->f_2434 = 0;
    this->f_2424 = 0;
    this->f_2428 = 0;
    this->f_242c = 0;
    this->f_2430 = 0;
}
