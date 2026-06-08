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
    self->f_4 = 0;
    self->f_0 = 0;
    void *wps = operator_new(0xc);
    ArrayWaypoint_ctor(wps);
    self->f_c = wps;
    void *timesArr = operator_new(0xc);
    ArrayInt_ctor(timesArr);
    self->f_10 = targets;
    self->f_14 = timesArr;
    for (int i = 0; i < count; i += 3)
        ArrayAddInt(*times++, self->f_14);
    for (int i = 0; i < count; i += 3) {
        void *wp = operator_new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], self);
        ArrayAddWaypoint(wp, self->f_c);
    }
    return self;
}
