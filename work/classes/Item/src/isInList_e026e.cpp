#include "class.h"

__attribute__((minsize))
bool Item::isInList(int index, int amount, Array *items)
{
    if (items != 0) {
        volatile Array *v = items;
        uint32_t size = v->size;
        for (uint32_t i = 0; i < size; i++) {
            Item *item = static_cast<Item **>(v->data)[i];
            if (item->index == index && amount <= item->amount) {
                return true;
            }
        }
    }
    return false;
}
