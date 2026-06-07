#include "class.h"
extern "C" Item* operatorNewItem(unsigned int sz);
extern "C" void Item_ctor3(Item *it, void *a, void *b, void *c);
extern "C" void *ItemDtor(Item *it);
extern "C" void operatorDelete(void *p);
extern "C" void Ship_recomputeAfterSlots(Ship *self);
extern "C" void setMods(Ship *self, Array<int> *mods) {
    self->mods = mods;
    if (mods != 0) {
        for (unsigned int i = 0; i < mods->size; i = i + 1) {
            int v = mods->data[i];
            bool isDev = v == 2;
            if (isDev) {
                v = self->numAddedDeviceSlots;
            }
            if (isDev && v == 0) {
                self->slots[3] = self->slots[3] + 1;
                self->numAddedDeviceSlots = self->numAddedDeviceSlots + 1;
                Item *it = operatorNewItem(0x48);
                Item_ctor3(it, 0, 0, 0);
                ArrayAdd<Item*>(it, *self->equipment);
                Item **slot = &self->equipment->data[self->equipment->size - 1];
                if (*slot != 0) {
                    operatorDelete(ItemDtor(*slot));
                    slot = &self->equipment->data[self->equipment->size - 1];
                }
                *slot = 0;
            }
        }
    }
    return Ship_recomputeAfterSlots(self);
}
