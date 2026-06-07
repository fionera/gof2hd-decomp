#include "class.h"

// KIPlayer's vtable slot at +0x28 writes its current position (3 floats) to *out.
typedef void (*GetPosFn)(void *out);

static inline void getPos(void *kip, void *out)
{
    (*(*(GetPosFn **)kip + 0xa))(out);
}

// Route::getWaypoint(int) -> snap the waypoint's stored coords to its docking target's position.
// The on-stack position buffer makes the compiler emit the stack-protector canary.
extern "C" Waypoint *Route_getWaypointAt(Route *self, int index)
{
    char pos[12];
    Waypoint *wp = 0;
    if ((int)F<Array<Waypoint *> *>(self, 0xc)->length > index &&
        (wp = F<Array<Waypoint *> *>(self, 0xc)->data[index]) != 0) {
        void *k = F<Array<void *> *>(self, 0x10)->data[index];
        if (k != 0) {
            getPos(k, pos);
            *(int32_t *)((char *)wp + 0x124) = (int)*(float *)(pos + 0);
            getPos(F<Array<void *> *>(self, 0x10)->data[index], pos);
            *(int32_t *)((char *)wp + 0x128) = (int)*(float *)(pos + 4);
            getPos(F<Array<void *> *>(self, 0x10)->data[index], pos);
            *(int32_t *)((char *)wp + 0x12c) = (int)*(float *)(pos + 8);
        }
    }
    return wp;
}
