#include "class.h"
extern "C" void *ArrayItemDtor(Array<Item*> *a);
extern "C" void operatorDelete(void *p);
extern "C" void removeAllCargo(Ship *self) {
    if (self->cargo != 0) {
        void *p = ArrayItemDtor(self->cargo);
        operatorDelete(p);
    }
    self->cargo = 0;
}
