#include "class.h"

extern "C" uint8_t Achievements_hasMedal(Achievements *self, int index, int value)
{
    return F<int *>(self, 0x0)[index] == value;
}
