#include "class.h"

struct Item;
extern "C" void *Array_Item_dtor(void *arr);
extern "C" void Station_operator_delete(void *p);
extern "C" void *Station_operator_new(uint32_t sz);
extern "C" void Array_Item_ctor(void *arr);
extern "C" void ArraySetLength_Item(uint32_t len, void *arr);
extern "C" void *Item_clone(Item *item);

// Station::setItems(Array<Item*>*, bool) — this in r0, items in r1, deep in r2.
extern "C" void Station_setItems(Station *self, uint32_t *items, bool deep)
{
    void *old = self->f_28;
    if (old != 0)
        Station_operator_delete(Array_Item_dtor(old));
    self->f_28 = 0;
    if (items == 0 || !deep) {
        self->f_28 = items;
    } else {
        void *na = Station_operator_new(0xc);
        Array_Item_ctor(na);
        self->f_28 = na;
        ArraySetLength_Item(items[0], na);
        for (uint32_t i = 0; i < items[0]; i++) {
            void *cloned = Item_clone(((Item **)items[1])[i]);
            ((void **)((uint32_t *)self->f_28)[1])[i] = cloned;
        }
    }
}
