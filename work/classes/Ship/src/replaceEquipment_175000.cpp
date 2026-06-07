#include "class.h"
extern "C" void Ship_recomputeAfterSlots(void);
extern "C" void replaceEquipment(Ship *self, Array<Item*> *eq) {
    if (eq != 0) {
        int *slots = self->slots;
        unsigned int sum = slots[0] + slots[3] + slots[1] + slots[2];
        if (sum < eq->size) {
            int extra = eq->size - sum;
            self->numAddedDeviceSlots = extra;
            slots[3] = extra + slots[3];
        }
    }
    self->equipment = eq;
    return Ship_recomputeAfterSlots();
}
