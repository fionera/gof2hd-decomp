#include "class.h"

extern "C" void *Route_dtor(Route *r);
extern "C" void operator_delete(void *p);

extern "C" void KIPlayer_setRoute(KIPlayer *self, Route *route)
{
    Route *old = *(Route **)((char *)self + 0x6c);
    if (old != 0) {
        operator_delete(Route_dtor(old));
    }
    *(Route **)((char *)self + 0x6c) = 0;
    if (route != 0) {
        *(Route **)((char *)self + 0xb4) = route;
    }
    *(Route **)((char *)self + 0x6c) = route;
}
