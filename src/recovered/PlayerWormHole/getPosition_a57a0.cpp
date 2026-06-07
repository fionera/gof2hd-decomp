#include "class.h"

struct PlayerWormHole {
    Vector getPosition();
};

Vector PlayerWormHole::getPosition()
{
    float x = (float)F<int>(this, 0x124);
    float y = (float)F<int>(this, 0x128);
    float z = (float)F<int>(this, 0x12c);
    Vector result = {x, y, z};
    return result;
}
