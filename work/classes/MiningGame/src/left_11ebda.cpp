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

    void left(float amount);
};

void MiningGame::left(float amount)
{
    F(this, 0x00) = amount * 3.0f;
}
