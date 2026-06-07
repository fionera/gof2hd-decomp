#include "class.h"
extern "C" int Item_getIndex(Item *it);
extern "C" int Item_getAmount(Item *it);
extern "C" int hasEquipment(Ship *self, int index, int amount) {
    Array<Item*> *e = self->equipment;
    if (e != 0) {
        for (unsigned int i = 0; i < e->size; i = i + 1) {
            Item *it = e->data[i];
            if (it != 0 && Item_getIndex(it) == index &&
                Item_getAmount(self->equipment->data[i]) >= amount) {
                return 1;
            }
            e = self->equipment;
        }
    }
    return 0;
}
