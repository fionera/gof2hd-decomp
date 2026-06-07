#include "class.h"

extern "C" Waypoint *Route_getWaypointAt(Route *self, int index);

// Route::getLastWaypoint() -> last waypoint in the path.
extern "C" Waypoint *Route_getLastWaypoint(Route *self)
{
    return Route_getWaypointAt(self, (int)F<Array<Waypoint *> *>(self, 0xc)->length - 1);
}
