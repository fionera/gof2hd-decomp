#include "class.h"
extern "C" int SolarSystem_getIndex(int system);
bool Status::inBlackMarketSystem() {
    if (inAlienOrbit()) return false;
    return SolarSystem_getIndex(system) == 0x19;
}
