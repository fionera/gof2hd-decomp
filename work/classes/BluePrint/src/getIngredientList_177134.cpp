#include "class.h"

extern "C" Array<int> *Item_getIngredients(Item *it);
// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::getIngredientList() -> tail-call Item::getIngredients(itemDB[index]).
extern "C" Array<int> *BluePrint_getIngredientList(BluePrint *self)
{
    int idx = F<int32_t>(self, 0x20);
    Item **data = *(Item ***)((char *)*(void **)gItemDB + 0x4);
    return Item_getIngredients(data[idx]);
}
