#include "class.h"
extern "C" void *Ship_dtor(Ship *);
extern "C" void op_delete(void *);
void Status::setShip(Ship *s) {
    if (ship != 0) {
        op_delete(Ship_dtor(ship));
        ship = 0;
    }
    ship = s;
}
