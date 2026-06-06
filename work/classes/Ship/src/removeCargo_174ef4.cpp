#include "class.h"
// Ship::removeCargo(int) -> tail-calls Ship::removeCargo(int,int) with amount = 9999999
extern "C" void Ship_removeCargo3(Ship *self, int idx, int amount);
extern "C" void removeCargo(Ship *self, int idx) {
    return Ship_removeCargo3(self, idx, 9999999);
}
