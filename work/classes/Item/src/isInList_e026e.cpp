#include "class.h"

bool Item::isInList(int index, int amount, Array *items)
{
    if (items == 0) {
        return false;
    }

    uint32_t size = items->size;
    volatile Array *volatileItems = items;
    uint32_t i = 0;

    goto test;
loop:
    {
        Item *item = static_cast<Item **>(volatileItems->data)[i];
        if (item->index == index && amount <= item->amount) {
            return true;
        }
        i++;
    }

test:
    if (i < size) {
        goto loop;
    }

    return false;
}
