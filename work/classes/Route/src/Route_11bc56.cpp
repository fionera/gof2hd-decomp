#include "class.h"

extern "C" void *operator_new(unsigned int sz);
extern "C" void ArrayWaypoint_ctor(void *a);   // Array<Waypoint*>::Array
extern "C" void ArrayInt_ctor(void *a);        // Array<int>::Array
extern "C" void ArrayAddInt(int v, void *a);
extern "C" void Waypoint_ctor(void *wp, int x, int y, int z, Route *route);
extern "C" void ArrayAddWaypoint(void *wp, void *a);

// Route::Route(int *coords, Array<KIPlayer*> *targets, int *times, int count)
extern "C" Route *_ZN5RouteC2EPiPvPii(Route *self, int *coords, void *targets, int *times, int count)
{
    F<uint8_t>(self, 0x4) = 0;
    F<int32_t>(self, 0x0) = 0;
    void *wps = operator_new(0xc);
    ArrayWaypoint_ctor(wps);
    F<void *>(self, 0xc) = wps;
    void *timesArr = operator_new(0xc);
    ArrayInt_ctor(timesArr);
    F<void *>(self, 0x10) = targets;
    F<void *>(self, 0x14) = timesArr;
    for (int i = 0; i < count; i += 3)
        ArrayAddInt(*times++, F<void *>(self, 0x14));
    for (int i = 0; i < count; i += 3) {
        void *wp = operator_new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], self);
        ArrayAddWaypoint(wp, F<void *>(self, 0xc));
    }
    return self;
}
