#include "class.h"
extern "C" Array<int>* operatorNewModsArr(unsigned int sz);
extern "C" void Array_int_ctor(Array<int> *a);
extern "C" Item* operatorNewItem(unsigned int sz);
extern "C" void Item_ctor3(Item *it, void *a, void *b, void *c);
extern "C" void *ItemDtor(Item *it);
extern "C" void operatorDelete(void *p);
extern "C" void Ship_recomputeAfterSlots(Ship *self);
extern "C" void addMod(Ship *self, int mod) {
    Array<int> *m = self->mods;
    if (m == 0) {
        m = operatorNewModsArr(0xc);
        Array_int_ctor(m);
        self->mods = m;
    }
    unsigned int sz = m->size;
    int found = 0;
    unsigned int i = 0;
    while (!found && i < sz) {
        found = m->data[i] == mod;
        i = i + 1;
    }
    if (found) {
        return;
    }
    ArrayAdd<int>(mod, *m);
    int dev = 0;
    if (mod == 2) {
        dev = self->numAddedDeviceSlots;
    }
    if (mod == 2 && dev == 0) {
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
    return Ship_recomputeAfterSlots(self);
}
