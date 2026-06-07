#include "class.h"
extern "C" int Item_getSort(Item *it);
extern int *gShopRoot;
extern "C" Item* getFirstEquipmentOfSort(Ship *self, int sort) {
    Array<Item*> *e = self->equipment;
    if (e != 0) {
        for (unsigned int i = 0; i < e->size; i = i + 1) {
            Item *it = e->data[i];
            if (it != 0) {
                int s = Item_getSort(it);
                e = self->equipment;
                if (s == sort) {
                    return e->data[i];
                }
            }
        }
        if (sort == 0x15 && (self->index == 0x31 || self->index == 0x2c)) {
            int *q = gShopRoot;
            return *(Item **)(*(int *)((char *)q + 4) + 0x17c);
        }
    }
    return 0;
}
