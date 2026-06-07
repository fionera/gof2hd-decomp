#include "class.h"

extern "C" void Achievements_resetPirateKills(Achievements *self)
{
    F<int>(self, 0x10) = 0;
}
