#include "class.h"
// Ship::addCargo(Array<Item*>*) -> combine then tail-call addCargo(self, combined)
extern "C" Array<Item*>* Item_combineItems(Array<Item*> *a, Array<Item*> *b);
extern "C" void Ship_addCargo2(Ship *self, Array<Item*> *items);
extern "C" void addCargo(Ship *self, Array<Item*> *items) {
    return Ship_addCargo2(self, Item_combineItems(self->cargo, items));
}
