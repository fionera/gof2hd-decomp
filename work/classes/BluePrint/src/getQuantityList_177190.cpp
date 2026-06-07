#include "class.h"

extern "C" Array<int> *Item_getQuantities(Item *it);
// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::getQuantityList() -> tail-call Item::getQuantities(itemDB[index]).
extern "C" Array<int> *BluePrint_getQuantityList(BluePrint *self)
{
    int idx = F<int32_t>(self, 0x20);
    Item **data = *(Item ***)((char *)*(void **)gItemDB + 0x4);
    return Item_getQuantities(data[idx]);
}
