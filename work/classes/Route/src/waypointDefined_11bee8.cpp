#include "class.h"

// Route::waypointDefined() -> whether the waypoint array has been allocated.
extern "C" bool Route_waypointDefined(Route *self)
{
    return F<void *>(self, 0xc) != 0;
}
