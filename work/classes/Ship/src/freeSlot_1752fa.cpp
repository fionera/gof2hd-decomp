#include "class.h"
extern "C" int Item_equals(Item *it, Item *other);
extern "C" void Ship_recomputeAfterSlots(Ship *self);
extern "C" void freeSlot(Ship *self, Item *item, int slot) {
    unsigned int i = 0;
    while (i < self->equipment->size) {
        Item *it = self->equipment->data[i];
        if (it != 0) {
            int r = Item_equals(it, item);
            if ((unsigned int)slot == i && r != 0) {
                self->equipment->data[slot] = 0;
                break;
            }
        }
        i = i + 1;
    }
    return Ship_recomputeAfterSlots(self);
}
