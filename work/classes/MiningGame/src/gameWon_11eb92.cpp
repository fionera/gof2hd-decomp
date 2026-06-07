#include "class.h"

struct MiningGame {
    uint8_t gameWon();
};

uint8_t MiningGame::gameWon()
{
    return UC(this, 0x81);
}
