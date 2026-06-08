#include "class.h"

struct Ship;
extern "C" int Ship_getIndex(Ship *ship);

// Station::hasShip(int) — this in r0, index in r1.
extern "C" uint32_t Station_hasShip(Station *self, int index)
{
    uint32_t *arr = self->f_2c;
    if (arr != 0) {
        for (uint32_t i = 0; i < arr[0]; i++) {
            Ship *sh = ((Ship **)arr[1])[i];
            if (sh != 0) {
                if (Ship_getIndex(sh) == index)
                    return 1;
                arr = self->f_2c;
            }
        }
    }
    return 0;
}
