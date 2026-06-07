#include "class.h"

struct MiningGame {
    void down(float amount);
};

void MiningGame::down(float amount)
{
    F(this, 0x04) = amount * 3.0f;
}
