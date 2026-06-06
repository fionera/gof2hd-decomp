#include "class.h"

__attribute__((target("-movt")))
int Item::getAttribute(int attribute) {
    Array *array = attributes;
    volatile Array *volatileArray = array;
    uint32_t size = array->size;
    uint32_t index = 0;
    int result = (int)0xc5997825;

    goto test;
loop:
    {
        int *data = (int *)volatileArray->data;
        int value = data[index];
        if (value == attribute) {
            return data[index + 1];
        }
        index += 2;
    }

test:
    if (index < size) {
        goto loop;
    }

    return result;
}
