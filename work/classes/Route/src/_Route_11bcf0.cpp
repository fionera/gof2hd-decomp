#include "class.h"

extern "C" void ArrayReleaseClasses_Waypoint(Array<Waypoint *> *a);
extern "C" void *ArrayWaypoint_dtor(Array<Waypoint *> *a);
extern "C" void *ArrayKIPlayer_dtor(Array<KIPlayer *> *a);
extern "C" void *ArrayInt_dtor(Array<int> *a);
extern "C" void operator_delete(void *p);

// Route::~Route() (D2). Returns this.
extern "C" void *_ZN5RouteD2Ev(Route *self)
{
    Array<Waypoint *> *wps = F<Array<Waypoint *> *>(self, 0xc);
    if (wps != 0) {
        ArrayReleaseClasses_Waypoint(wps);
        Array<Waypoint *> *wps2 = F<Array<Waypoint *> *>(self, 0xc);
        if (wps2 != 0)
            operator_delete(ArrayWaypoint_dtor(wps2));
    }
    F<Array<Waypoint *> *>(self, 0xc) = 0;

    Array<KIPlayer *> *tgt = F<Array<KIPlayer *> *>(self, 0x10);
    if (tgt != 0)
        operator_delete(ArrayKIPlayer_dtor(tgt));
    F<Array<KIPlayer *> *>(self, 0x10) = 0;

    Array<int> *times = F<Array<int> *>(self, 0x14);
    if (times != 0)
        operator_delete(ArrayInt_dtor(times));
    F<Array<int> *>(self, 0x14) = 0;

    return self;
}
