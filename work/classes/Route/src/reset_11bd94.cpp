#include "class.h"

extern "C" void Waypoint_reset(Waypoint *wp);

// Route::reset() -> reset every waypoint and rewind to the start.
extern "C" void Route_reset(Route *self)
{
    for (uint32_t i = 0; i < F<Array<Waypoint *> *>(self, 0xc)->length; i++)
        Waypoint_reset(F<Array<Waypoint *> *>(self, 0xc)->data[i]);
    self->f_0 = 0;
}
