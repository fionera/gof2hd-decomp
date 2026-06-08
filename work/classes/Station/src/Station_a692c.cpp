#include "class.h"

extern "C" void *String_default_ctor(void *s);         // String::String() -> this
extern "C" void String_assign(void *dst, void *src);   // operator=(String*, String*)

// Station::Station(String, int, int, int, int)
// r0=this, r1=String* param, r2=p3, r3=p4, [stack]=p5, p6.
extern "C" Station *Station_ctor(Station *self, void *name,
                                 int p3, int p4, int p5, int p6)
{
    void *s = String_default_ctor(self);
    String_assign(s, name);
    self->f_c = p3;
    self->f_10 = p4;
    self->f_20 = p5;
    self->f_18 = p6;
    self->f_1c = 0;
    self->f_24 = 0;
    self->f_28 = 0;
    self->f_2c = 0;
    self->f_30 = 0;
    return self;
}
