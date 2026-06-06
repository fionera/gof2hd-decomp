#include "class.h"
extern "C" int SolarSystem_getIndex(int system);

// Returns 1 only when stranded in the supernova solar system before the late campaign.
int Status::inSupernovaSystem() {
    if (__builtin_expect((long)inAlienOrbit(), 1)) return 0;
    int idx = SolarSystem_getIndex(system);
    int result = 0;
    if (idx == 0x1b && currentCampaignMission < 0x9e) result = 1;
    return result;
}
