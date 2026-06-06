#include "class.h"
extern "C" int SolarSystem_getTextureIndex(int system);
bool Status::inFogSkyboxOrbit() {
    if (inAlienOrbit()) return false;
    if (SolarSystem_getTextureIndex(system) == 0x11) return true;
    return SolarSystem_getTextureIndex(system) == 0x12;
}
