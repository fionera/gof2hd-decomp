#include "class.h"

struct MiningGame {
    int getOreAmount();
};

int MiningGame::getOreAmount()
{
    return (int)F(this, 0x24);
}
