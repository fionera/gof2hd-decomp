#include "class.h"
extern "C" int Item_getTotalPrice(Item *it);
extern "C" int getEquipmentValue(Ship *self) {
    unsigned int i = 0;
    int total = 0;
    for (; i < self->equipment->size; i = i + 1) {
        Item *it = self->equipment->data[i];
        if (it != 0) {
            total += Item_getTotalPrice(it);
        }
    }
    return total;
}
