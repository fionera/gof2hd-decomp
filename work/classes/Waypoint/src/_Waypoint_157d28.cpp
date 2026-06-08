#include "class.h"

extern "C" void *KIPlayer_dtor(KIPlayer *self);
extern "C" void operator_delete(void *ptr);

extern "C" void _ZN8WaypointD0Ev(Waypoint *self)
{
    return operator_delete(KIPlayer_dtor((KIPlayer *)self));
}
