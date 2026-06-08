#include "class.h"

static const int kPirateStations[4] = { 0, 0, 0, 0 };

extern "C" uint32_t Station_getPirateStationIndex(Station *self)
{
    uint32_t i = 0;
    while (true) {
        if (i > 3)
            return 0xffffffff;
        if (kPirateStations[i] == self->f_c)
            return i;
        i++;
    }
}
