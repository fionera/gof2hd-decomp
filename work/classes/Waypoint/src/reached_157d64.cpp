#include "class.h"

struct Waypoint {
    void reached();
};

void Waypoint::reached()
{
    F<uint16_t>(this, 0x130) = 0x101;
}
