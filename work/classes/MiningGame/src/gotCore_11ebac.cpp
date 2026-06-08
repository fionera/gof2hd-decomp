#include "class.h"

struct MiningGame {
    // @portable-fields
    unsigned char _pad_0[136];
    void* f_88; // 0x88
    void* f_8c; // 0x8c
    void* f_90; // 0x90
    void* f_94; // 0x94
    unsigned char _pad_98[56];
    void* f_d0; // 0xd0

    uint8_t gotCore();
};

uint8_t MiningGame::gotCore()
{
    return UC(this, 0x83);
}
