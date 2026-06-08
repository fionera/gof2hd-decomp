#include "class.h"

static const int kPirateStations[4] = { 0, 0, 0, 0 };

extern "C" uint32_t Station_getPirateStationIndex(Station *self)
{
    uint32_t i = 0;
    while (true) {
        if (i > 3)
            return 0xffffffff;
        if (kPirateStations[i] == F<int>(self, 0xc))
            return i;
        i++;
    }
}
