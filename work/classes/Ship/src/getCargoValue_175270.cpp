#include "class.h"
extern "C" int Item_getTotalPrice(Item *it);
extern "C" int getCargoValue(Ship *self) {
    int total = 0;
    Array<Item*> *c = self->cargo;
    if (c != 0) {
        for (unsigned int i = 0; i < c->size; i = i + 1) {
            Item *it = c->data[i];
            if (it != 0) {
                int v = Item_getTotalPrice(it);
                c = self->cargo;
                total = total + v;
            }
        }
    }
    return total;
}
