#include "class.h"

struct Ship;
extern "C" void *Station_operator_new(uint32_t sz);
extern "C" void Array_Ship_ctor(void *arr);
extern "C" int Ship_equals(Ship *a, Ship *b);
extern "C" void ArrayAdd_Ship(Ship *ship, void *arr);   // tail-called veneer

// Station::addShip(Ship*) — this in r0, ship in r1.
extern "C" void Station_addShip(Station *self, Ship *ship)
{
    uint32_t *arr = self->f_2c;
    if (arr == 0) {
        arr = (uint32_t *)Station_operator_new(0xc);
        Array_Ship_ctor(arr);
        self->f_2c = arr;
    } else {
        uint32_t n = arr[0];
        if (n != 0) {
            for (uint32_t i = 0; i < n; i++) {
                if (Ship_equals(((Ship **)arr[1])[i], ship) != 0) {
                    if ((int)i >= 0)
                        return;
                    arr = self->f_2c;
                    break;
                }
                arr = self->f_2c;
                n = arr[0];
            }
        }
    }
    ArrayAdd_Ship(ship, arr);
}
