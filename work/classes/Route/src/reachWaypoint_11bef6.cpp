#include "class.h"

extern "C" void Waypoint_reset(Waypoint *wp);
extern "C" void Waypoint_setActive(Waypoint *wp, bool active);
extern "C" void Waypoint_activate(Waypoint *wp);   // veneer tail-call target

// Route::reachWaypoint(int) -> advance to the given waypoint, wrapping/resetting when looping.
extern "C" void Route_reachWaypoint(Route *self, int index)
{
    Array<Waypoint *> *wps = F<Array<Waypoint *> *>(self, 0xc);
    uint32_t len = wps->length;
    if (F<int32_t>(self, 0x0) < (int)(len - 1)) {
        F<int32_t>(self, 0x0) = index + 1;
    } else if (F<uint8_t>(self, 0x4) != 0) {
        F<int32_t>(self, 0x0) = 0;
        for (uint32_t i = 0; i < len; i++) {
            Waypoint_reset(F<Array<Waypoint *> *>(self, 0xc)->data[i]);
            len = F<Array<Waypoint *> *>(self, 0xc)->length;
        }
        Waypoint_setActive(F<Array<Waypoint *> *>(self, 0xc)->data[0], true);
    }
    Waypoint_setActive(F<Array<Waypoint *> *>(self, 0xc)->data[index], false);
    return Waypoint_activate(F<Array<Waypoint *> *>(self, 0xc)->data[index]);
}
