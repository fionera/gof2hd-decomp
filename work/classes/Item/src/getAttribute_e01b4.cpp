#include "class.h"

__attribute__((minsize))
int Item::getAttribute(int attribute)
{
    volatile Array *v = attributes;
    int result = (int)0xc5997825;
    uint32_t size = v->size;
    for (uint32_t index = 0; index < size; index += 2) {
        int *data = (int *)v->data;
        if (data[index] == attribute) {
            return data[index + 1];
        }
    }
    return result;
}
