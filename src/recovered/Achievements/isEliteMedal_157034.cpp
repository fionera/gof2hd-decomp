#include "class.h"

extern "C" uint8_t Achievements_isEliteMedal(Achievements *self, int index)
{
    return index > 0x23;
}
