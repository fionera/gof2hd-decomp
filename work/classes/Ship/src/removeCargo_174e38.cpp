#include "class.h"
// Ship::removeCargo(Item*) -> tail-call removeCargo(self, item->getIndex(), item->getAmount())
extern "C" int Item_getIndex(Item *it);
extern "C" int Item_getAmount(Item *it);
extern "C" int Ship_removeCargo3(Ship *self, int index, int amount);
extern "C" void removeCargo(Ship *self, Item *item) {
    int idx = Item_getIndex(item);
    int amt = Item_getAmount(item);
    Ship_removeCargo3(self, idx, amt);
}
