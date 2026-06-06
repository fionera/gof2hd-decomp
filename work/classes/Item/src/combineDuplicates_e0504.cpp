#include "class.h"

extern "C" void ArrayRemove_Item(Item *item, Array *items);

__attribute__((minsize))
void Item::combineDuplicates(Array *items)
{
    if (items != 0) {
        volatile Array *v = items;
        uint32_t size = v->size;
        for (uint32_t i = 0; i != size; i++) {
            for (uint32_t j = i + 1; j != size; j++) {
                Item *right = static_cast<Item **>(v->data)[j];
                Item *left = static_cast<Item **>(v->data)[i];
                if (left->index == right->index) {
                    left->amount += right->amount;
                    right->amount = 0;
                    left->stationAmount += right->stationAmount;
                    right->stationAmount = 0;
                }
            }
        }
        for (uint32_t i = 0; i < size; i++) {
            Item *item = static_cast<Item **>(v->data)[i];
            if (item->amount == 0 && item->stationAmount == 0) {
                ArrayRemove_Item(item, items);
                size = v->size;
            }
        }
    }
}
