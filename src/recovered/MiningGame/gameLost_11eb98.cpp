#include "class.h"

struct MiningGame {
    uint8_t gameLost();
};

uint8_t MiningGame::gameLost()
{
    return UC(this, 0x82);
}
