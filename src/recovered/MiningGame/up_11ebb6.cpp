#include "class.h"

struct MiningGame {
    void up(float amount);
};

void MiningGame::up(float amount)
{
    F(this, 0x04) = amount * 3.0f;
}
