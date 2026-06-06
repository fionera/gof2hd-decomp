#include "class.h"
extern "C" int Item_getType(Item *it);
extern "C" int hasCargoType(Ship *self, int type) {
    unsigned int *p = (unsigned int *)self->cargo;
    if (p != 0) {
        for (unsigned int i = 0; i < *p; i = i + 1) {
            Item *it = *(Item **)(p[1] + i * 4);
            if (it != 0) {
                int t = Item_getType(it);
                if (t == type) return 1;
                p = (unsigned int *)self->cargo;
            }
        }
    }
    return 0;
}
