#include "class.h"

// 4-entry price table loaded PC-relative (table contents in local rodata).
static const int kModPriceTable[4] = { 0, 0, 0, 0 };

extern "C" int Agent_getModPricePercentage(Agent *self)
{
    uint32_t i = F<uint32_t>(self, 0x94);
    if (i < 4)
        return kModPriceTable[i];
    return 0x28;
}
