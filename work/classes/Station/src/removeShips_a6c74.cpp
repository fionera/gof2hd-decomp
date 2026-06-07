#include "class.h"

struct Ship;
extern "C" void ArrayReleaseClasses_Ship(void *arr);
extern "C" void *Array_Ship_dtor(void *arr);
extern "C" void Station_operator_delete(void *p);

extern "C" void Station_removeShips(Station *self)
{
    void *arr = F<void *>(self, 0x2c);
    if (arr != 0) {
        ArrayReleaseClasses_Ship(arr);
        void *a2 = F<void *>(self, 0x2c);
        if (a2 != 0)
            Station_operator_delete(Array_Ship_dtor(a2));
    }
    F<void *>(self, 0x2c) = 0;
}
