#include "class.h"

struct Waypoint {
    void reset();
};

extern "C" void Player_setActive(Player *self, bool active);

void Waypoint::reset()
{
    Player *player = F<Player *>(this, 0x4);
    F<uint8_t>(this, 0x130) = 0;
    return Player_setActive(player, false);
}
