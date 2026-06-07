#include "class.h"

struct MiningGame {
    void left(float amount);
};

void MiningGame::left(float amount)
{
    F(this, 0x00) = amount * 3.0f;
}
