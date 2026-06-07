#include "class.h"

// SolarSystem::getStationEnumIndex(int) — index of matching station in array at +0x38, or -1.
extern "C" uint32_t SolarSystem_getStationEnumIndex(SolarSystem *self, int idx)
{
    uint32_t *arr = F<uint32_t *>(self, 0x38);
    for (uint32_t i = 0; i < arr[0]; i++) {
        if (((int *)arr[1])[i] == idx)
            return i;
    }
    return 0xffffffff;
}
