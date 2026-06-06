#include "class.h"
extern "C" int Item_getType(Item *it);
extern "C" int hasSecondaryWeapons(Ship *self) {
    if (self->slots[1] != 0) {
        unsigned int *p = (unsigned int *)self->equipment;
        if (p != 0) {
            unsigned int n = *p;
            if (n != 0) {
                for (unsigned int i = 0; i < n; i = i + 1) {
                    Item *it = *(Item **)(p[1] + i * 4);
                    if (it != 0) {
                        int t = Item_getType(it);
                        if (t == 1) return 1;
                        p = (unsigned int *)self->equipment;
                        n = *p;
                    }
                }
            }
        }
    }
    return 0;
}
