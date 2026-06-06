#include "class.h"
extern "C" int getSlotTypes(Ship *self) {
    int n = 0;
    int i = 0;
    while (i != 4) {
        int idx = i;
        i = i + 1;
        if (0 < self->slots[idx]) n = n + 1;
    }
    return n;
}
