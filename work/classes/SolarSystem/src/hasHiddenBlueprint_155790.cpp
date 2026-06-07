#include "class.h"

extern "C" int SolarSystem_stationIsInSystem_int(SolarSystem *self, int idx);

// Hidden-blueprint station-index table (PC-relative global).
extern const int kBlueprintStations[5] __attribute__((visibility("hidden")));
// Singleton: global holds P; **P is the status/base struct pointer kept in r8.
extern void *gBlueprintRoot __attribute__((visibility("hidden")));

// SolarSystem::hasHiddenBlueprint() — a blueprint station in this system with its flag cleared.
extern "C" int SolarSystem_hasHiddenBlueprint(SolarSystem *self)
{
    char *base = *(char **)gBlueprintRoot;
    uint32_t i = 0;
    while (true) {
        if (i > 4)
            return 0;
        if (SolarSystem_stationIsInSystem_int(self, kBlueprintStations[i]) != 0) {
            char *flags = *(char **)(*(char **)(base + 0x58) + 4);
            if (flags[i] == 0)
                return 1;
        }
        i++;
    }
}
