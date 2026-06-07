#include "class.h"

extern "C" Array<int> *BluePrint_getIngredientList(BluePrint *self);

// BluePrint::getRemainingAmount(int) -> remaining counter for the ingredient with the given index.
extern "C" int BluePrint_getRemainingAmount(BluePrint *self, int item)
{
    Array<int> *il = BluePrint_getIngredientList(self);
    Array<int> *counters = F<Array<int> *>(self, 0x0);
    for (uint32_t i = 0; i < counters->length; i++) {
        if (il->data[i] == item)
            return counters->data[i];
    }
    return 0;
}
