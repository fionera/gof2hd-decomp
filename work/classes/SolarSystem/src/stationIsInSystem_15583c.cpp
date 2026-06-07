#include "class.h"

struct Station;
extern "C" int Station_getIndex(Station *st);
// SolarSystem::stationIsInSystem(int) overload, tail-called.
extern "C" int SolarSystem_stationIsInSystem_int(SolarSystem *self, int idx);

// SolarSystem::stationIsInSystem(Station*) — null -> 0, else dispatch by station index.
// NOTE: near-match only. The body (getIndex + tail-call to the int overload) is
// byte-identical, but clang -Oz schedules the trivial `st==0 -> return 0` path
// before the prologue (cbz; ...; bx lr), whereas the target saves `this` in r4
// first and shares the pop epilogue. Resistant to source-level restructuring.
extern "C" int SolarSystem_stationIsInSystem(SolarSystem *self, Station *st)
{
    if (st != 0)
        return SolarSystem_stationIsInSystem_int(self, Station_getIndex(st));
    return 0;
}
