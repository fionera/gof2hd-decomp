#include "class.h"

// Route::translate(Vector const&) -> shift every waypoint's coords by the given vector.
extern "C" void Route_translate(Route *self, const Vector &v)
{
    Array<Waypoint *> *wps = F<Array<Waypoint *> *>(self, 0xc);
    float vx = v.x, vy = v.y, vz = v.z;
    for (uint32_t i = 0; i != wps->length; i++) {
        Waypoint *wp = wps->data[i];
        *(int32_t *)((char *)wp + 0x124) = (int)((float)*(int32_t *)((char *)wp + 0x124) + vx);
        *(int32_t *)((char *)wp + 0x128) = (int)((float)*(int32_t *)((char *)wp + 0x128) + vy);
        *(int32_t *)((char *)wp + 0x12c) = (int)((float)*(int32_t *)((char *)wp + 0x12c) + vz);
    }
}
