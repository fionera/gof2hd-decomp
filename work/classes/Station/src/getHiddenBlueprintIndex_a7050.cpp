#include "class.h"

static const int kHiddenBlueprints[5] = { 0, 0, 0, 0, 0 };

extern "C" uint32_t Station_getHiddenBlueprintIndex(Station *self)
{
    uint32_t i = 0;
    while (true) {
        if (i > 4)
            return 0xffffffff;
        if (kHiddenBlueprints[i] == F<int>(self, 0xc))
            return i;
        i++;
    }
}
