#include "class.h"

extern "C" Array<int> *BluePrint_getIngredientList(BluePrint *self);
extern "C" int Item_getSinglePrice(Item *it);
// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::getIngredientsValue() -> sum over ingredients of (remaining * single price).
extern "C" int BluePrint_getIngredientsValue(BluePrint *self)
{
    Array<int> *il = BluePrint_getIngredientList(self);
    int total = 0;
    if (il != 0) {
        for (uint32_t i = 0; i < il->length; i++) {
            Item **data = *(Item ***)((char *)*(void **)gItemDB + 0x4);
            int price = Item_getSinglePrice(data[il->data[i]]);
            total += F<Array<int> *>(self, 0x0)->data[i] * price;
        }
    }
    return total;
}
