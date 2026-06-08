#include "class.h"

extern "C" int BluePrint_getIngredientsValue(BluePrint *self);
extern "C" int Item_getMaxPrice(Item *it);
// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::getAutoCompletionPrice():
//   item 0xd2 (raw materials slot) -> ingredient value plus a fixed sentinel bonus;
//   otherwise -> batch * item max price, scaled by 1.25.
extern "C" int BluePrint_getAutoCompletionPrice(BluePrint *self)
{
    int idx = F<int32_t>(self, 0x20);
    if (idx == 0xd2)
        return BluePrint_getIngredientsValue(self) + 0x1e8480;
    Item **data = *(Item ***)((char *)*(void **)gItemDB + 0x4);
    int maxPrice = Item_getMaxPrice(data[idx]);
    return (int)((float)(F<int32_t>(self, 0x24) * maxPrice) * 1.25f);
}
