#include "class.h"
extern "C" void Ship_recomputeAfterSlots(void);
extern "C" void freeAllSlots(Ship *self) {
    for (unsigned int i = 0; i < self->equipment->size; i = i + 1) {
        Item **data = self->equipment->data;
        if (data[i] != 0) {
            data[i] = 0;
        }
    }
    return Ship_recomputeAfterSlots();
}
