#include "class.h"

// SolarSystem::stationIsInSystem(int) — scan station-index array at +0x38.
extern "C" int SolarSystem_stationIsInSystem_int(SolarSystem *self, int idx)
{
    uint32_t *arr = F<uint32_t *>(self, 0x38);
    uint32_t n = arr[0];
    for (uint32_t i = 0; i < n; i++) {
        if (((int *)arr[1])[i] == idx)
            return 1;
    }
    return 0;
}
