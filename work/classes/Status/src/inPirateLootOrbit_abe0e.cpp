#include "class.h"
extern "C" int SolarSystem_getIndex(int system);
bool Status::inPirateLootOrbit() {
    if (inAlienOrbit()) return false;
    if (SolarSystem_getIndex(system) == 0x20) return true;
    return SolarSystem_getIndex(system) == 0x21;
}
