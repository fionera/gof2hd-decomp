#include "class.h"

extern "C" void Mission_setProductionGoods(Mission *self, int a, int b) {
    F<int>(self, 0x68) = a;
    F<int>(self, 0x6c) = b;
}
