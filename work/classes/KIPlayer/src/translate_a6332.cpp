#include "class.h"

extern "C" void AEGeometry_translate(void *geom, const Vector &v);
extern "C" void Route_translate(Route *route, const Vector &v);

extern "C" void KIPlayer_translate(KIPlayer *self, const Vector &v)
{
    AEGeometry_translate(*(void **)((char *)self + 0x8), v);
    Route *route = *(Route **)((char *)self + 0x6c);
    if (route == 0) {
        return;
    }
    return Route_translate(route, v);
}
