#include "class.h"

struct MiningGame {
    uint8_t gotCore();
};

uint8_t MiningGame::gotCore()
{
    return UC(this, 0x83);
}
