#include "class.h"
extern "C" void Ship_recomputeAfterSlots(void);
extern "C" void setEquipment(Ship *self, Item *item) {
    Array<Item*> *eq = self->equipment;
    unsigned int i = 0;
    Item **data;
    Item *v;
    do {
        if (i >= eq->size) goto done;
        data = eq->data;
        v = data[i];
        i = i + 1;
    } while (v != 0);
    data[i - 1] = item;
done:
    return Ship_recomputeAfterSlots();
}
