#include "class.h"

extern "C" void Achievements_incKills(Achievements *self)
{
    F<int>(self, 0x8) += 1;
}
