#include "class.h"

struct Status;
struct Station;
// Status::getStation() and Station::getIndex(Station*) — resolved blx targets.
extern "C" Station *Status_getStation(Status *s);
extern "C" int Station_getIndex(Station *st);

// Singleton slot: global variable holds a Status*; the Status's first member is
// passed to getStation. Single-level pointer so the access is:
//   ldr r1,[lit]; add r1,pc; ldr r1,[r1] (= the Status*); ldr r0,[r1] (= field 0)
extern Status *gStatusOrbit __attribute__((visibility("hidden")));

// SolarSystem::currentOrbitHasWarpGate() — orbit field at +0x30 vs current station index.
extern "C" bool SolarSystem_currentOrbitHasWarpGate(SolarSystem *self)
{
    int orbit = F<int>(self, 0x30);
    Station *st = Status_getStation(*(Status **)gStatusOrbit);
    return orbit == Station_getIndex(st);
}
