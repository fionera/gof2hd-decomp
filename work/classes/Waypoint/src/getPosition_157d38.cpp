#include "class.h"

struct Waypoint {
    Vector getPosition();
};

Vector Waypoint::getPosition()
{
    float x = (float)F<int32_t>(this, 0x124);
    float y = (float)F<int32_t>(this, 0x128);
    float z = (float)F<int32_t>(this, 0x12c);
    Vector result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}
