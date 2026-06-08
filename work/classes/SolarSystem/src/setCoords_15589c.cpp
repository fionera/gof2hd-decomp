#include "class.h"

// SolarSystem::setCoords(int, int) — strd r1,r2,[r0,#0x24]; bx lr
extern "C" void SolarSystem_setCoords(SolarSystem *self, int x, int y)
{
    F<int>(self, 0x24) = x;
    F<int>(self, 0x28) = y;
}
