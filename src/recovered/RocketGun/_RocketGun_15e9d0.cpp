#include "class.h"

extern "C" __attribute__((visibility("hidden"))) void *RocketGun_vtable;
extern "C" void *Array_Matrix_dtor(void *p);
extern "C" void *Array_int_dtor(void *p);
extern "C" void operator_delete(void *p);
extern "C" void *RocketGun_base_dtor(RocketGun *self);

extern "C" void *_ZN9RocketGunD1Ev(RocketGun *self)
{
    *(void **)self = (char *)RocketGun_vtable + 8;

    void *a0 = F<void *>(self, 0xd8);
    if (a0 != 0)
        operator_delete(Array_Matrix_dtor(a0));

    void *a1 = F<void *>(self, 0xdc);
    F<void *>(self, 0xd8) = 0;
    if (a1 != 0)
        operator_delete(Array_int_dtor(a1));

    void *a2 = F<void *>(self, 0xe0);
    F<void *>(self, 0xdc) = 0;
    if (a2 != 0)
        operator_delete(Array_int_dtor(a2));

    F<void *>(self, 0xe0) = 0;
    return RocketGun_base_dtor(self);
}
