#include "class.h"
extern "C" void ArrayReleaseClassesItem(Array<Item*> *a);
extern "C" void *ArrayItemDtor(Array<Item*> *a);
extern "C" void *ArrayIntDtor(Array<int> *a);
extern "C" void operatorDelete(void *p);

extern "C" Ship *_ZN4ShipD2Ev(Ship *self) {
    Array<Item*> *a = self->equipment;
    if (a != 0) {
        if (a->size != 0) {
            ArrayReleaseClassesItem(a);
            a = self->equipment;
            if (a == 0) goto skip1;
        }
        operatorDelete(ArrayItemDtor(a));
    }
skip1:
    a = self->cargo;
    self->equipment = 0;
    if (a != 0) {
        if (a->size != 0) {
            ArrayReleaseClassesItem(a);
            a = self->cargo;
            if (a == 0) goto skip2;
        }
        operatorDelete(ArrayItemDtor(a));
    }
skip2:
    self->cargo = 0;
    if (self->slots != 0) {
        operatorDelete(self->slots);
    }
    self->slots = 0;
    if (self->mods != 0) {
        operatorDelete(ArrayIntDtor(self->mods));
    }
    self->mods = 0;
    return self;
}
