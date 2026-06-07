#include "class.h"

extern "C" void Achievements_setMedal(Achievements *self, int index, int value)
{
    F<int *>(self, 0x0)[index] = value;
}
