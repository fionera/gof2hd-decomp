#include "class.h"

struct Ship;
extern "C" void ArrayReleaseClasses_Ship(void *arr);
extern "C" void *Array_Ship_dtor(void *arr);
extern "C" void Station_operator_delete(void *p);
extern "C" void *Station_operator_new(uint32_t sz);
extern "C" void Array_Ship_ctor(void *arr);
extern "C" void ArraySetLength_Ship(uint32_t len, void *arr);
extern "C" void *Ship_clone(Ship *ship);

// Station::setShips(Array<Ship*>*, bool) — this in r0, ships in r1, deep in r2.
extern "C" void Station_setShips(Station *self, uint32_t *ships, bool deep)
{
    void *old = F<void *>(self, 0x2c);
    if (old != 0) {
        ArrayReleaseClasses_Ship(old);
        void *o2 = F<void *>(self, 0x2c);
        if (o2 != 0)
            Station_operator_delete(Array_Ship_dtor(o2));
    }
    F<void *>(self, 0x2c) = 0;
    if (ships == 0 || !deep) {
        F<uint32_t *>(self, 0x2c) = ships;
    } else {
        void *na = Station_operator_new(0xc);
        Array_Ship_ctor(na);
        F<void *>(self, 0x2c) = na;
        ArraySetLength_Ship(ships[0], na);
        for (uint32_t i = 0; i < ships[0]; i++) {
            void *cloned = Ship_clone(((Ship **)ships[1])[i]);
            ((void **)((uint32_t *)F<uint32_t *>(self, 0x2c))[1])[i] = cloned;
        }
    }
}
