#include "class.h"

static const int kHiddenBlueprints[5] = { 0, 0, 0, 0, 0 };

extern "C" uint32_t Station_getHiddenBlueprintIndex(Station *self)
{
    uint32_t i = 0;
    while (true) {
        if (i > 4)
            return 0xffffffff;
        if (kHiddenBlueprints[i] == self->f_c)
            return i;
        i++;
    }
}
