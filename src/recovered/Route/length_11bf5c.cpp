#include "class.h"

// Route::length() -> number of waypoints.
extern "C" int Route_length(Route *self)
{
    return (int)F<Array<Waypoint *> *>(self, 0xc)->length;
}
