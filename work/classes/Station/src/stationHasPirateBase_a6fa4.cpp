#include "class.h"

static const int kPirateStations[4] = { 0, 0, 0, 0 };
extern char **const gPirateBaseSingleton __attribute__((visibility("hidden")));

// Station::stationHasPirateBase() — this in r0.
extern "C" uint32_t Station_stationHasPirateBase(Station *self)
{
    char *base = *gPirateBaseSingleton;       // r12 = *(*(global))
    uint32_t i = 0;
    while (true) {
        if (i > 3)
            return 0;
        if (kPirateStations[i] == self->f_c) {
            char *flags = *(char **)(*(char **)(base + 0x4c) + 4);
            if (flags[i] == 0)
                return 1;
        }
        i++;
    }
}
