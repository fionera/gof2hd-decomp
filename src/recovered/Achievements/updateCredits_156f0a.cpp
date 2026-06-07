#include "class.h"

extern "C" void Achievements_updateCredits(Achievements *self, int value)
{
    if (F<int>(self, 0x1c) < value)
        F<int>(self, 0x1c) = value;
}
