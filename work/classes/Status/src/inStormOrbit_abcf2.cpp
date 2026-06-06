#include "class.h"
extern "C" int SolarSystem_getTextureIndex(int system);
bool Status::inStormOrbit() {
    if (__builtin_expect((long)inAlienOrbit(), 0)) return false;
    if (__builtin_expect(currentCampaignMission < 0x5a, 0)) return false;
    if (inSupernovaSystem() != 0) return true;
    if (SolarSystem_getTextureIndex(system) == 0x10) return true;
    return SolarSystem_getTextureIndex(system) == 0x12;
}
