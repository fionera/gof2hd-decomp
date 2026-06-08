#include "class.h"

extern "C" void operator_delete_array(void *p);
extern "C" void Galaxy_ArrayReleaseClasses_SolarSystem(void *arr);
extern "C" void *Galaxy_Array_SolarSystem_dtor(void *arr);
extern "C" void operator_delete(void *p);

extern "C" Galaxy *_ZN6GalaxyD2Ev(Galaxy *self)
{
    operator_delete_array(P(self, 0x0));
    void *systems = P(self, 0x4);
    P(self, 0x0) = 0;
    Galaxy_ArrayReleaseClasses_SolarSystem(systems);
    if (P(self, 0x4) != 0) {
        operator_delete(Galaxy_Array_SolarSystem_dtor(P(self, 0x4)));
    }
    P(self, 0x4) = 0;
    return self;
}
