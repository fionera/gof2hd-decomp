#include "class.h"

extern "C" void Achievements_incPirateKills(Achievements *self)
{
    F<int>(self, 0x10) += 1;
}
