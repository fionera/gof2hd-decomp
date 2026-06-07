#include "class.h"

struct MiningGame {
    void right(float amount);
};

void MiningGame::right(float amount)
{
    F(this, 0x00) = amount * 3.0f;
}
