#include "class.h"
extern "C" int Item_getIndex(Item *it);
extern "C" int Item_getAmount(Item *it);
extern "C" void Item_changeAmount(Item *it, int delta);
extern "C" Array<Item*>* Item_extractItems(Array<Item*> *a, bool flag);
extern "C" void setCargo(Ship *self, Array<Item*> *cargo);
extern "C" int removeCargo(Ship *self, int index, int amount) {
    Array<Item*> *c = self->cargo;
    if (c != 0) {
        unsigned int i = 0;
        int amt;
        for (;;) {
            if (i >= c->size) goto done;
            if (Item_getIndex(c->data[i]) == index) break;
            c = self->cargo;
            i = i + 1;
        }
        Item_changeAmount(self->cargo->data[i], -amount);
        amt = Item_getAmount(self->cargo->data[i]);
        c = self->cargo;
        if (amt < 1) {
            c = Item_extractItems(c, true);
            setCargo(self, c);
            return 1;
        }
    done:
        setCargo(self, c);
    }
    return 0;
}
