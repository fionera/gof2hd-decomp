#include "class.h"

extern "C" void Achievements_resetNewMedals(Achievements *self)
{
    int *newMedals = F<int *>(self, 0x4);
    int i;
    for (i = 0; i != 0x2d; i = i + 1)
        newMedals[i] = 0;
    F<uint8_t>(self, 0x18) = 0;
    F<int>(self, 0x8) = 0;
    F<int>(self, 0xc) = 0;
    F<int>(self, 0x10) = 0;
    F<int>(self, 0x14) = 0;
}
