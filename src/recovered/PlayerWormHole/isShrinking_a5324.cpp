#include "class.h"

struct PlayerWormHole {
    bool isShrinking();
};

bool PlayerWormHole::isShrinking()
{
    return F<int>(this, 0x150) > 60000;
}
