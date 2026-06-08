#include "class.h"

struct Item;
extern "C" void *Station_operator_new(uint32_t sz);
extern "C" void Array_Item_ctor(void *arr);
extern "C" int Item_equals(Item *a, Item *b);
extern "C" int Item_getAmount(Item *item);
extern "C" void Item_addAmount(Item *found, int amount);  // tail-called veneer
extern "C" void ArrayAdd_Item(Item *item, void *arr);     // tail-called veneer

// Station::addItem(Item*) — this in r0, item in r1.
extern "C" void Station_addItem(Station *self, Item *item)
{
    uint32_t *arr = F<uint32_t *>(self, 0x28);
    if (arr == 0) {
        arr = (uint32_t *)Station_operator_new(0xc);
        Array_Item_ctor(arr);
        F<uint32_t *>(self, 0x28) = arr;
    } else {
        uint32_t n = arr[0];
        if (n != 0) {
            for (uint32_t i = 0; i < n; i++) {
                if (Item_equals(((Item **)arr[1])[i], item) != 0) {
                    if ((int)i < 0)
                        break;
                    uint32_t *cur = F<uint32_t *>(self, 0x28);
                    Item *found = ((Item **)cur[1])[i];
                    Item_addAmount(found, Item_getAmount(item));
                    return;
                }
                arr = F<uint32_t *>(self, 0x28);
                n = arr[0];
            }
        }
    }
    ArrayAdd_Item(item, arr);
}
