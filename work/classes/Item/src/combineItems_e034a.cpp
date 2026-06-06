#include "class.h"

extern "C" void Array_Item_ctor(Array *array);
extern "C" void ArraySetLength_Item(uint32_t length, Array *array);

Array *Item::combineItems(Array *items, Array *stationItems)
{
    Array *result = stationItems;
    if (items != 0) {
        result = items;
        if (stationItems != 0) {
            Array *stationCopy = static_cast<Array *>(operator new(sizeof(Array)));
            Array_Item_ctor(stationCopy);
            ArraySetLength_Item(stationItems->size, stationCopy);

            uint32_t stationCount = stationItems->size;
            for (uint32_t i = 0; stationCount != i; i++) {
                static_cast<Item **>(stationCopy->data)[i] =
                    static_cast<Item **>(stationItems->data)[i];
            }

            uint32_t copyCount = stationCopy->size;
            uint32_t remaining = copyCount;
            uint32_t itemCount = items->size;
            for (uint32_t itemIndex = 0; itemCount != itemIndex; itemIndex++) {
                for (uint32_t stationIndex = 0; copyCount != stationIndex; stationIndex++) {
                    Item **copyData = static_cast<Item **>(stationCopy->data);
                    Item *stationItem = copyData[stationIndex];
                    if (stationItem != 0) {
                        Item *item = static_cast<Item **>(items->data)[itemIndex];
                        if (item->index == stationItem->index) {
                            remaining--;
                            item->amount = stationItem->amount + item->amount;
                            copyData[stationIndex] = 0;
                        }
                    }
                }
            }

            if (static_cast<int>(remaining) > 0) {
                Array *unmatched = static_cast<Array *>(operator new(sizeof(Array)));
                Array_Item_ctor(unmatched);
                ArraySetLength_Item(remaining, unmatched);

                uint32_t totalCopied = stationCopy->size;
                uint32_t unmatchedIndex = 0;
                for (uint32_t i = 0; totalCopied != i; i++) {
                    Item *item = static_cast<Item **>(stationCopy->data)[i];
                    if (item != 0) {
                        static_cast<Item **>(unmatched->data)[unmatchedIndex] = item;
                        unmatchedIndex++;
                    }
                }

                result = static_cast<Array *>(operator new(sizeof(Array)));
                Array_Item_ctor(result);
                ArraySetLength_Item(items->size + unmatched->size, result);

                uint32_t itemCountForCopy = items->size;
                for (uint32_t i = 0; itemCountForCopy != i; i++) {
                    static_cast<Item **>(result->data)[i] =
                        static_cast<Item **>(items->data)[i];
                }

                uint32_t unmatchedCount = unmatched->size;
                for (uint32_t i = 0; unmatchedCount != i; i++) {
                    static_cast<Item **>(result->data)[itemCountForCopy + i] =
                        static_cast<Item **>(unmatched->data)[i];
                }
            }
        }
    }
    return result;
}
