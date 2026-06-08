#include "class.h"

extern "C" Route *Route_clone(Route *self);
extern "C" void Waypoint_reached(Waypoint *wp);

// Route::getExactClone() -> a clone whose reached-waypoints are marked, sharing the current index.
extern "C" Route *Route_getExactClone(Route *self)
{
    Route *result = Route_clone(self);
    for (uint32_t i = 0; i < F<Array<Waypoint *> *>(result, 0xc)->length; i++) {
        if (*(uint8_t *)((char *)F<Array<Waypoint *> *>(self, 0xc)->data[i] + 0x130) != 0)
            Waypoint_reached(F<Array<Waypoint *> *>(result, 0xc)->data[i]);
    }
    result->f_0 = self->f_0;
    return result;
}
