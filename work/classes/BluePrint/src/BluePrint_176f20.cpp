#include "class.h"

extern "C" void String_ctor(void *s);                  // AbyssEngine::String::String()
extern "C" int Item_getType(Item *it);
extern "C" Array<int> *Item_getQuantities(Item *it);
extern "C" Array<int> *Item_getIngredients(Item *it);
extern "C" void *operator_new(unsigned int sz);
extern "C" void ArrayInt_ctor(void *a);                // Array<int>::Array()
extern "C" void ArraySetLengthInt(uint32_t n, Array<int> *a);
// Hidden PC-relative pointer-to-the-item-database root.
extern void *const gItemDB __attribute__((visibility("hidden")));

// BluePrint::BluePrint(int item)
extern "C" BluePrint *_ZN9BluePrintC2Ei(BluePrint *self, int item)
{
    String_ctor((char *)self + 0x14);
    F<int32_t>(self, 0x20) = item;
    Item **db = *(Item ***)((char *)*(void **)gItemDB + 0x4);
    Item *it = db[item];
    int type = Item_getType(it);
    F<int32_t>(self, 0x10) = -1;
    F<int32_t>(self, 0x24) = (type == 1) ? 10 : 1;
    Array<int> *quantities = Item_getQuantities(it);
    F<Array<int> *>(self, 0x0) = 0;
    if (Item_getIngredients(it) != 0) {
        void *p = operator_new(0xc);
        ArrayInt_ctor(p);
        F<void *>(self, 0x0) = p;
        ArraySetLengthInt(Item_getIngredients(it)->length, F<Array<int> *>(self, 0x0));
        Array<int> *arr = F<Array<int> *>(self, 0x0);
        for (uint32_t i = 0; i < arr->length; i++)
            arr->data[i] = quantities->data[i];
    }
    F<int32_t>(self, 0xc) = 0;
    F<uint8_t>(self, 0x8) = 0;
    F<int32_t>(self, 0x4) = 0;
    F<int32_t>(self, 0x28) = F<int32_t>(self, 0x24);
    return self;
}
