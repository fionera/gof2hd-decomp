#include "class.h"

bool Radar::stationLocked()
{
    void *station = this->f_24;
    if (station != 0) {
        return station->f_71 != 0;
    }
    return false;
}
