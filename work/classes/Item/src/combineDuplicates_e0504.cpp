#include "class.h"

extern "C" void ArrayRemove(Item *item, Array *items);

void Item::combineDuplicates(Array *items)
{
    if (items != 0) {
        volatile Array *array = items;
        uint32_t size = array->size;
        uint32_t next;
        uint32_t i = 0;

        while ((next = i), next != size) {
            for (uint32_t j = next + 1; (i = next + 1), size != j; j++) {
                volatile Item *right = static_cast<Item **>(array->data)[j];
                volatile Item *left = static_cast<Item **>(array->data)[next];

                if (left->index == right->index) {
                    left->amount += right->amount;
                    right->amount = 0;
                    left->stationAmount += right->stationAmount;
                    right->stationAmount = 0;
                }
            }
        }

        for (i = 0; i < size; i++) {
            volatile Item *item = static_cast<Item **>(array->data)[i];
            if (item->amount == 0 && item->stationAmount == 0) {
                ArrayRemove(const_cast<Item *>(item), items);
                size = array->size;
            }
        }
    }
}
