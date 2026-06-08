#include "class.h"

extern "C" void Mission_setProductionGoods(Mission *self, int a, int b) {
    self->f_68 = a;
    self->f_6c = b;
}
