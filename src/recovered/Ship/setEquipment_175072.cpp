#include "class.h"
extern "C" void Ship_setEquipment1(Ship *self, Item *item);
extern "C" void setEquipment(Ship *self, Array<Item*> *items) {
    for (unsigned int i = 0; i < items->size; i = i + 1) {
        Ship_setEquipment1(self, items->data[i]);
    }
}
