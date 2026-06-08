#include "class.h"

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

extern "C" void Array_Item_ctor(Array *array);
extern "C" void *Array_Item_dtor(Array *array);
extern "C" void ArraySetLength_Item(uint32_t size, Array *array);

Array *Item::mixItems(Array *items, Array *stationItems)
{
    uint32_t itemCount = items ? items->size : 0;
    int stationCount = stationItems ? static_cast<int>(stationItems->size) : 0;

    Array *mixed = static_cast<Array *>(operator new(sizeof(Array)));
    Array_Item_ctor(mixed);
    ArraySetLength_Item(stationCount + itemCount, mixed);

    if (static_cast<int>(itemCount) >= 1 && stationCount == 0) {
        uint32_t i = 0;
        while (i < items->size) {
            Item *item = static_cast<Item **>(items->data)[i];
            static_cast<Item **>(mixed->data)[i] = item->makeItem(item->amount);
            i++;
        }
    } else if (itemCount == 0 && stationCount > 0) {
        uint32_t i = 0;
        while (i < stationItems->size) {
            Item *item = static_cast<Item **>(stationItems->data)[i];
            Item *copy = item->makeItem(0);
            static_cast<Item **>(mixed->data)[i] = copy;
            copy->stationAmount = item->amount;
            i++;
        }
    } else if ((stationCount | static_cast<int>(itemCount)) == 0) {
        mixed = 0;
    } else {
        uint32_t i = 0;
        while (i < items->size) {
            Item *item = static_cast<Item **>(items->data)[i];
            static_cast<Item **>(mixed->data)[i] = item->makeItem(item->amount);
            i++;
        }

        uint32_t stationIndex = 0;
        while (stationIndex < stationItems->size) {
            uint32_t mixedIndex = 0;
            while (mixedIndex < mixed->size) {
                Item *mixedItem = static_cast<Item **>(mixed->data)[mixedIndex];
                Item *stationItem = static_cast<Item **>(stationItems->data)[stationIndex];
                if (mixedItem == 0) {
                    Item *copy = stationItem->makeItem(0);
                    itemCount++;
                    static_cast<Item **>(mixed->data)[mixedIndex] = copy;
                    copy->stationAmount = stationItem->amount;
                    break;
                }
                if (stationItem->index == mixedItem->index) {
                    Item *copy = stationItem->makeItem(mixedItem->amount);
                    static_cast<Item **>(mixed->data)[mixedIndex] = copy;
                    copy->stationAmount = stationItem->amount;
                    break;
                }
                mixedIndex++;
            }
            stationIndex++;
        }

        Array *trimmed = static_cast<Array *>(operator new(sizeof(Array)));
        Array_Item_ctor(trimmed);
        ArraySetLength_Item(itemCount, trimmed);

        int copyIndex = 0;
        while (copyIndex < static_cast<int>(itemCount)) {
            static_cast<Item **>(trimmed->data)[copyIndex] = static_cast<Item **>(mixed->data)[copyIndex];
            copyIndex++;
        }

        operator delete(Array_Item_dtor(mixed));

        uint32_t size = trimmed->size;
        bool done = true;
        uint32_t sortIndex = 1;
        do {
            while (sortIndex < size) {
                Item **data = static_cast<Item **>(trimmed->data);
                Item *right = data[sortIndex];
                Item *left = data[sortIndex - 1];
                if (right->index < left->index) {
                    data[sortIndex - 1] = right;
                    done = false;
                    static_cast<Item **>(trimmed->data)[sortIndex] = left;
                }
                sortIndex++;
            }
            bool again = !done;
            sortIndex = 1;
            done = true;
            if (!again) {
                break;
            }
        } while (true);

        Item::combineDuplicates(trimmed);
        mixed = trimmed;
    }

    return mixed;
}
