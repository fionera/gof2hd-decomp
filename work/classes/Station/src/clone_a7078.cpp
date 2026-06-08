#include "class.h"

extern "C" void *Station_operator_new(uint32_t sz);
extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" void String_dtor(void *s);
extern "C" Station *Station_ctor(Station *self, void *name, int p3, int p4, int p5, int p6);

// Station::clone() — this in r0, returns a new Station copy.
extern "C" Station *Station_clone(Station *self)
{
    Station *n = (Station *)Station_operator_new(0x34);
    char tmp[12];
    String_copy_ctor(tmp, self, false);
    Station_ctor(n, tmp, self->f_c, self->f_10,
                 self->f_20, self->f_18);
    String_dtor(tmp);
    return n;
}
