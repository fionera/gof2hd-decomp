#include "class.h"
extern "C" Ship* clone(Ship *self);
extern "C" void makeShip(Ship *self, int v) {
    Ship *s = clone(self);
    if (-1 < v) s->price = v;
}
