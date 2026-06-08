#include "class.h"

bool Radar::stationLocked()
{
    void *station = F<void *>(this, 0x24);
    if (station != 0) {
        return F<uint8_t>(station, 0x71) != 0;
    }
    return false;
}
