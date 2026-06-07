#include "class.h"
extern "C" int Item_getType(Item *it);
extern "C" int addEquipment(Ship *self, Item *item) {
    int type = Item_getType(item);
    int cap = self->slots[type];
    if (cap < 1) {
        return 0;
    }
    int base = 0;
    for (int k = 0; k < type; k = k + 1) {
        base = base + self->slots[k];
    }
    int end = base + cap;
    while (base < end) {
        Item *cur = self->equipment->data[base];
        base = base + 1;
        if (cur == 0) {
            self->equipment->data[base - 1] = item;
            return 1;
        }
    }
    return 0;
}
