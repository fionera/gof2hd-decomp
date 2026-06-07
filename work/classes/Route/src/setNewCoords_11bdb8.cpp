#include "class.h"

// Route::setNewCoords(Vector) -> overwrite the first waypoint's target coordinates.
extern "C" void Route_setNewCoords(Route *self, float x, float y, float z)
{
    Waypoint *wp = F<Array<Waypoint *> *>(self, 0xc)->data[0];
    *(int32_t *)((char *)wp + 0x124) = (int)x;
    *(int32_t *)((char *)wp + 0x128) = (int)y;
    *(int32_t *)((char *)wp + 0x12c) = (int)z;
}
