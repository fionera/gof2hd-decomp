#include "class.h"

extern "C" Waypoint *Route_getWaypointAt(Route *self, int index);

// Route::getWaypoint() -> waypoint at the current index.
extern "C" Waypoint *Route_getWaypoint(Route *self)
{
    return Route_getWaypointAt(self, F<int32_t>(self, 0x0));
}
