#include "class.h"
extern "C" int Item_equals(Item *it, Item *other);
extern "C" void removeEquipment(Ship *self, Item *item) {
    Array<Item*> *eq = self->equipment;
    if (eq != 0) {
        for (unsigned int i = 0; i < eq->size; i = i + 1) {
            Item *it = eq->data[i];
            if (it != 0) {
                int r = Item_equals(it, item);
                eq = self->equipment;
                if (r != 0) {
                    eq->data[i] = 0;
                    return;
                }
            }
        }
    }
}
