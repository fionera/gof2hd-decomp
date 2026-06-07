#include "class.h"
extern "C" int Item_getIndex(Item *it);
extern "C" int Item_getAmount(Item *it);
extern "C" int hasCargo(Ship *self, int index, int amount) {
    Array<Item*> *c = self->cargo;
    if (c != 0) {
        for (unsigned int i = 0; i < c->size; i = i + 1) {
            Item *it = c->data[i];
            if (it != 0 && Item_getIndex(it) == index &&
                Item_getAmount(self->cargo->data[i]) >= amount) {
                return 1;
            }
            c = self->cargo;
        }
    }
    return 0;
}
