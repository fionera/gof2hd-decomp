#include "class.h"

void *operator new(__SIZE_TYPE__ size);

extern "C" void Array_Item_ctor(Array *array);
extern "C" void ArrayAdd_Item(Item *item, Array *array);
extern "C" void ArrayAdd_Array(Array *items, Array *array);

__attribute__((minsize)) void Item::fabricate(Item *item, Array *items, int amount)
{
    volatile uint32_t *ingredients = reinterpret_cast<volatile uint32_t *>(item->ingredients);
    Array *quantities = item->quantities;
    volatile uint32_t *itemsRaw = reinterpret_cast<volatile uint32_t *>(items);
    uint32_t i = 0;
    uint32_t count = ingredients[0];

    goto check;
loop:
    {
        uint32_t itemCount = itemsRaw[0];
        uint32_t j = 0;
    inner:
        if (j < itemCount) {
            int *candidate = reinterpret_cast<int **>(itemsRaw[1])[j];
            j++;
            if (candidate[0] != reinterpret_cast<volatile int *>(ingredients[1])[i]) {
                goto inner;
            }
            candidate[13] -= reinterpret_cast<int *>(reinterpret_cast<uint32_t *>(quantities)[1])[i] * amount;
        }
    }
    i++;
check:
    if (i < count) {
        goto loop;
    }

    Array *made = static_cast<Array *>(operator new(sizeof(Array)));
    Array_Item_ctor(made);
    ArrayAdd_Item(item->makeItem(amount), made);
    return ArrayAdd_Array(items, made);
}
