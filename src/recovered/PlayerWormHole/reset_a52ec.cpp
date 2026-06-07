#include "class.h"

struct PlayerWormHole {
    void reset(bool shrinking);
};

void PlayerWormHole::reset(bool shrinking)
{
    int value = 0;
    if (shrinking)
        value = 59000;
    F<int>(this, 0x150) = value;
    F<int>(this, 0x154) = 0x1000;
}
