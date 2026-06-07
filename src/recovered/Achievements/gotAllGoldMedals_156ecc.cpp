#include "class.h"

extern "C" uint8_t Achievements_gotAllGoldMedals(Achievements *self)
{
    return F<uint8_t>(self, 0x21);
}
