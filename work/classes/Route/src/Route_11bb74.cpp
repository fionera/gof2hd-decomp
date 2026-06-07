#include "class.h"

extern "C" void *operator_new(unsigned int sz);
extern "C" void ArrayWaypoint_ctor(void *a);   // Array<Waypoint*>::Array
extern "C" void ArrayKIPlayer_ctor(void *a);   // Array<KIPlayer*>::Array
extern "C" void ArrayInt_ctor(void *a);        // Array<int>::Array
extern "C" int __aeabi_idiv(int n, int d);
extern "C" void ArraySetLengthKIPlayer(uint32_t n, void *a);
extern "C" void ArraySetLengthInt(uint32_t n, void *a);
extern "C" void Waypoint_ctor(void *wp, int x, int y, int z, Route *route);
extern "C" void ArrayAddWaypoint(void *wp, void *a);

// Route::Route(int *coords, int count) -- count triples of (x,y,z) define the waypoints.
extern "C" Route *_ZN5RouteC2EPii(Route *self, int *coords, int count)
{
    F<uint8_t>(self, 0x4) = 0;
    F<int32_t>(self, 0x0) = 0;
    void *wps = operator_new(0xc);
    ArrayWaypoint_ctor(wps);
    F<void *>(self, 0xc) = wps;
    void *tgts = operator_new(0xc);
    ArrayKIPlayer_ctor(tgts);
    F<void *>(self, 0x10) = tgts;
    void *times = operator_new(0xc);
    ArrayInt_ctor(times);
    F<void *>(self, 0x14) = times;
    uint32_t n = __aeabi_idiv(count, 3);
    ArraySetLengthKIPlayer(n, F<void *>(self, 0x10));
    ArraySetLengthInt(n, F<void *>(self, 0x14));
    for (int i = 0; i < count; i += 3) {
        void *wp = operator_new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], self);
        ArrayAddWaypoint(wp, F<void *>(self, 0xc));
    }
    return self;
}
