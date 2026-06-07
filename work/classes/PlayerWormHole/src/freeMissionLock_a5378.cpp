#include "class.h"

struct PlayerWormHole {
    void freeMissionLock();
};

void PlayerWormHole::freeMissionLock()
{
    F<uint8_t>(this, 0x15c) = 0;
}
