#include "class.h"
extern "C" int Item_getType(Item *it);
extern "C" unsigned int getSlotPos(Ship *self, Item *item) {
    if (item == 0) {
        return 0xffffffff;
    }
    Array<Item*> *eq = self->equipment;
    unsigned int n = eq->size;
    unsigned int pos = 0xffffffff;
    for (unsigned int i = 0; i < n; i = i + 1) {
        if (eq->data[i] == item) { pos = i; break; }
    }
    for (int j = 0; j < Item_getType(item); j = j + 1) {
        pos = pos - self->slots[j];
    }
    return pos;
}
