#include "class.h"
extern "C" int Item_getIndex(Item *it);
extern "C" Item* getCargo(Ship *self, int index) {
    Array<Item*> *c = self->cargo;
    if (c == 0) return 0;
    for (unsigned int i = 0; i < c->size; i = i + 1) {
        Item *it = c->data[i];
        if (it != 0) {
            int idx = Item_getIndex(it);
            c = self->cargo;
            if (idx == index) {
                return c->data[i];
            }
        }
    }
    return 0;
}
