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

    ~MiningGame();
};

extern "C" void *MiningGame_Sprite_dtor(void *sprite);
extern "C" void MiningGame_operator_delete(void *ptr);

MiningGame::~MiningGame()
{
    void *sprite = this->f_94;
    if (sprite != 0) {
        MiningGame_operator_delete(MiningGame_Sprite_dtor(sprite));
    }
    this->f_94 = 0;
}
