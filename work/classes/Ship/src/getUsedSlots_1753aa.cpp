#include "class.h"
extern "C" int Item_getType(Item *it);
extern "C" int getUsedSlots(Ship *self, int type) {
    unsigned int i = 0;
    int n = 0;
    for (; i < self->equipment->size; i = i + 1) {
        Item *it = self->equipment->data[i];
        if (it != 0 && Item_getType(it) == type) {
            n = n + 1;
        }
    }
    return n;
}
