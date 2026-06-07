#include "class.h"

extern "C" void *gCloakRand;  // hidden PC-relative global (deref'd twice)
extern "C" int AERandom_nextInt(int rng);

extern "C" void PlayerFighter_cloak(PlayerFighter *self, int dur, bool b)
{
    unsigned v;
    if (dur > 0) {
        v = (unsigned)dur;
    } else {
        v = AERandom_nextInt(**(int **)&gCloakRand) + 5000;
    }
    F<uint8_t>(self, 0x2d0) = 1;
    F<uint32_t>(self, 0x2cc) = v + 4000;
    F<uint8_t>(self, 0x2d9) = b;
}
