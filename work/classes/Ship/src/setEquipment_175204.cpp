#include "class.h"
extern "C" int Item_getType(Item *it);
extern "C" void *ItemDtor(Item *it);
extern "C" void operatorDelete(void *p);
extern "C" void Ship_recomputeAfterSlots(Ship *self);
extern "C" void setEquipment(Ship *self, Item *item, int slot) {
    unsigned int idx = slot;
    for (int i = 0; i < Item_getType(item); i = i + 1) {
        idx = idx + self->slots[i];
    }
    if (idx >= self->equipment->size) {
        return;
    }
    Item **dst;
    Item *old = self->equipment->data[idx];
    if (old == 0) {
        dst = &self->equipment->data[idx];
    } else {
        operatorDelete(ItemDtor(old));
        dst = &self->equipment->data[idx];
    }
    *dst = 0;
    self->equipment->data[idx] = item;
    return Ship_recomputeAfterSlots(self);
}
