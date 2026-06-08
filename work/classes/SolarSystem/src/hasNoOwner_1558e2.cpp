#include "class.h"

// SolarSystem::hasNoOwner() — owner race at +0x18; map a contiguous range to a bitmask.
// (race-0x17) < 0xb ? (0x60b >> (x & 0xff)) & 1 : 0
extern "C" uint32_t SolarSystem_hasNoOwner(SolarSystem *self)
{
    uint32_t x = F<uint32_t>(self, 0x18) - 0x17;
    if (x < 0xb)
        return (0x60bU >> (x & 0x7ff)) & 1;
    return 0;
}
