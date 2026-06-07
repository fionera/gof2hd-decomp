#include "class.h"
extern "C" Array<Item*>* operatorNewArr(unsigned int sz);
extern "C" void Array_Item_ctor(Array<Item*> *a);
extern "C" Array<Item*>* getEquipment(Ship *self, int type) {
    if (type >= 4 || self->slots[type] == 0) {
        return 0;
    }
    Array<Item*> *result = operatorNewArr(0xc);
    Array_Item_ctor(result);
    ArraySetLength<Item*>(self->slots[type], *result);
    int base = 0;
    for (int i = 0; i < type; i = i + 1) {
        base = base + self->slots[i];
    }
    unsigned int j = 0;
    for (int i = base; i < self->slots[type] + base; i = i + 1) {
        if (j < result->size) {
            result->data[j] = self->equipment->data[i];
            j = j + 1;
        }
    }
    return result;
}
