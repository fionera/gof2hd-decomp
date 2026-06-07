#include "class.h"

struct Galaxy;
struct Station;
struct Status;
struct SolarSystem;
struct Systems { uint32_t n; SolarSystem **data; uint32_t cap; };

// Singletons held as pointer-to-pointer in hidden globals.
__attribute__((visibility("hidden"))) extern Galaxy **g_galaxy;
__attribute__((visibility("hidden"))) extern Status **g_status;
extern "C" Station *Galaxy_getStation(Galaxy *g, int idx);
extern "C" Systems *Galaxy_getSystems(Galaxy *g);
extern "C" Station *Status_getStation(Status *s);
extern "C" int Station_getSystem(Station *s);
extern "C" float Galaxy_distance(Galaxy *g, SolarSystem *a, SolarSystem *b);
extern "C" Station *Station_dtor(Station *s);
extern "C" void Station_dtor_finish(Station *s);

// Computes the distance (light years) from the player's current system to this
// mission's target station system, storing the truncated value at +0x60.
extern "C" void Mission_calcDistance(Mission *self) {
    Galaxy **gp = g_galaxy;
    Station *st = Galaxy_getStation(*gp, F<int>(self, 0x3c));
    Systems *sys = Galaxy_getSystems(*gp);
    Galaxy *g = *gp;
    int i1 = Station_getSystem(Status_getStation(*g_status));
    SolarSystem *a = sys->data[i1];
    int i2 = Station_getSystem(st);
    SolarSystem *b = sys->data[i2];
    F<int>(self, 0x60) = (int)Galaxy_distance(g, a, b);
    if (st != 0) {
        Station_dtor_finish(Station_dtor(st));
    }
}
