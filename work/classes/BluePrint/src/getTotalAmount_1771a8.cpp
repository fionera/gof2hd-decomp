#include "class.h"

extern "C" Array<int> *BluePrint_getIngredientList(BluePrint *self);
extern "C" Array<int> *BluePrint_getQuantityList(BluePrint *self);

// BluePrint::getTotalAmount(int) -> required total quantity for the given ingredient index.
extern "C" int BluePrint_getTotalAmount(BluePrint *self, int item)
{
    Array<int> *il = BluePrint_getIngredientList(self);
    Array<int> *ql = BluePrint_getQuantityList(self);
    for (uint32_t i = 0; i < ql->length; i++) {
        if (il->data[i] == item)
            return ql->data[i];
    }
    return 0;
}
