#include "class.h"
extern "C" int hasCargo(Ship *self, int type, int amount);
extern "C" int hasVolatileGoods(Ship *self) {
    if (hasCargo(self, 0xd1, 1) != 0) return 1;
    return hasCargo(self, 0xcc, 1);
}
