#include "class.h"
extern "C" int Item_getSort(Item *it);
extern "C" int slotAvailable(Ship *self, int sort) {
    if (sort != 0 && sort != 0xc) {
        for (unsigned int i = 0; i < self->equipment->size; i = i + 1) {
            Item *it = self->equipment->data[i];
            if (it != 0 && Item_getSort(it) == sort) {
                return 0;
            }
        }
    }
    return 1;
}
