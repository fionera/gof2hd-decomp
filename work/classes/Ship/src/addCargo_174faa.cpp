#include "class.h"
extern "C" Array<Item*>* operatorNewArray(unsigned int sz);
extern "C" void ArrayCtorItem(Array<Item*> *a);
extern "C" void Ship_addCargo2(Ship *self, Array<Item*> *items);
extern "C" void addCargo(Ship *self, Item *item) {
    Array<Item*> *a = operatorNewArray(0xc);
    ArrayCtorItem(a);
    ArrayAdd<Item*>(item, *a);
    Ship_addCargo2(self, a);
}
