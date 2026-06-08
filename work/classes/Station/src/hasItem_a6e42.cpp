#include "class.h"

struct Item;
extern "C" int Item_getIndex(Item *item);

// Station::hasItem(int) — this in r0, index in r1.
extern "C" uint32_t Station_hasItem(Station *self, int index)
{
    uint32_t *arr = self->f_28;
    if (arr != 0) {
        for (uint32_t i = 0; i < arr[0]; i++) {
            Item *it = ((Item **)arr[1])[i];
            if (it != 0) {
                if (Item_getIndex(it) == index)
                    return 1;
                arr = self->f_28;
            }
        }
    }
    return 0;
}
