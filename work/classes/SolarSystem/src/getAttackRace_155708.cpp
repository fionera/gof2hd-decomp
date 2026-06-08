#include "class.h"

// SolarSystem::getAttackRace() — field at 0x20 indexes a 4-entry table; >=4 -> 8.
// The table is a PC-relative global (visibility hidden -> add r1,pc form).
extern const int kAttackRaceTable[4] __attribute__((visibility("hidden")));

extern "C" int SolarSystem_getAttackRace(SolarSystem *self)
{
    uint32_t r = F<uint32_t>(self, 0x20);
    if (r < 4)
        return kAttackRaceTable[r];
    return 8;
}
