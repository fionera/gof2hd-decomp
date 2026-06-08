#include "class.h"

static const int kHiddenBlueprints[5] = { 0, 0, 0, 0, 0 };
extern char **const gHiddenBlueprintSingleton __attribute__((visibility("hidden")));

// Station::stationHasHiddenBlueprint(bool) — this in r0, flag in r1.
extern "C" uint32_t Station_stationHasHiddenBlueprint(Station *self, bool ignoreFound)
{
    char *base = *gHiddenBlueprintSingleton;
    uint32_t i = 0;
    while (true) {
        if (i > 4)
            return 0;
        if (kHiddenBlueprints[i] == self->f_c) {
            if (ignoreFound)
                return 1;
            char *flags = *(char **)(*(char **)(base + 0x58) + 4);
            if (flags[i] == 0)
                return 1;
        }
        i++;
    }
}
