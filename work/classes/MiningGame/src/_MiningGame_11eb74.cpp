#include "class.h"

struct MiningGame {
    ~MiningGame();
};

extern "C" void *MiningGame_Sprite_dtor(void *sprite);
extern "C" void MiningGame_operator_delete(void *ptr);

MiningGame::~MiningGame()
{
    void *sprite = P(this, 0x94);
    if (sprite != 0) {
        MiningGame_operator_delete(MiningGame_Sprite_dtor(sprite));
    }
    P(this, 0x94) = 0;
}
