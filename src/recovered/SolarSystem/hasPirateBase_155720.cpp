#include "class.h"

extern "C" int SolarSystem_stationIsInSystem_int(SolarSystem *self, int idx);

// Pirate-base station-index table (PC-relative global).
extern const int kPirateBaseStations[4] __attribute__((visibility("hidden")));
// Singleton: global holds P; **P is the status/base struct pointer kept in r8.
extern void *gPirateBaseRoot __attribute__((visibility("hidden")));

// SolarSystem::hasPirateBase() — a pirate station in this system with its flag cleared.
extern "C" int SolarSystem_hasPirateBase(SolarSystem *self)
{
    char *base = *(char **)gPirateBaseRoot;
    uint32_t i = 0;
    while (true) {
        if (i > 3)
            return 0;
        if (SolarSystem_stationIsInSystem_int(self, kPirateBaseStations[i]) != 0) {
            char *flags = *(char **)(*(char **)(base + 0x4c) + 4);
            if (flags[i] == 0)
                return 1;
        }
        i++;
    }
}
