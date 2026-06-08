#include "class.h"

struct Waypoint {
    void setActive(bool active);
};

extern "C" void Player_setActive(Player *self, bool active);

void Waypoint::setActive(bool active)
{
    return Player_setActive(F<Player *>(this, 0x4), active);
}
