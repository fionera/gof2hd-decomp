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
    F<int>(self, 0xc) = p3;
    F<int>(self, 0x10) = p4;
    F<int>(self, 0x20) = p5;
    F<int>(self, 0x18) = p6;
    F<uint8_t>(self, 0x1c) = 0;
    F<uint8_t>(self, 0x24) = 0;
    F<int>(self, 0x28) = 0;
    F<int>(self, 0x2c) = 0;
    F<int>(self, 0x30) = 0;
    return self;
}
