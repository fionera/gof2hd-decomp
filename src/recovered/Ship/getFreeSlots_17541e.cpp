#include "class.h"
extern "C" int getUsedSlots(Ship *self, int type);
extern "C" int getFreeSlots(Ship *self, int type) {
    int total = self->slots[type];
    int used = getUsedSlots(self, type);
    return total - used;
}
