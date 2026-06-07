#include "class.h"
extern "C" Ship *operatorNewShip(unsigned int sz);
extern "C" Ship *Ship_ctor_full(Ship *self, int index, int baseHP, int baseLoad, int value,
                                int s0, int s1, int s2, int s3, float handling);
extern "C" void addMod(Ship *self, int mod);
extern "C" Ship *clone(Ship *self) {
    Ship *s = operatorNewShip(0x80);
    int *slots = self->slots;
    Ship_ctor_full(s, self->index, self->baseHP, self->baseLoad, self->price,
                   slots[0], slots[1], slots[2], slots[3] - self->numAddedDeviceSlots,
                   self->handling * 1.6f);
    Array<int> *m = self->mods;
    if (m != 0) {
        for (unsigned int i = 0; i < m->size; i = i + 1) {
            addMod(s, m->data[i]);
            m = self->mods;
        }
    }
    return s;
}
