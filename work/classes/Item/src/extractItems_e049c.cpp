#include "class.h"

extern "C" void Array_Item_ctor(Array *array);
extern "C" void ArrayAdd_Item(Item *item, Array *array);

Array *Item::extractItems(Array *items, bool station)
{
    if (items == 0) {
        return 0;
    }

    Array *extracted = static_cast<Array *>(operator new(sizeof(Array)));
    Array_Item_ctor(extracted);

    for (uint32_t i = 0; i < items->size; i++) {
        Item *item = static_cast<Item **>(items->data)[i];
        int amount;
        if (station) {
            amount = item->amount;
        } else {
            amount = item->stationAmount;
        }
        if (amount > 0) {
            ArrayAdd_Item(item->makeItem(), extracted);
        }
    }

    if (extracted->size == 0) {
        extracted = 0;
    }

    return extracted;
}
