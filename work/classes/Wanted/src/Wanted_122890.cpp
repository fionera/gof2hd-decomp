#include "class.h"

// String::String(String* this) default ctor -> 0x6efbc (returns this in r0)
extern "C" Wanted *String_default_ctor(Wanted *self);
// String::operator=(String* this, const String* rhs) -> 0x6f2b0
extern "C" void String_assign(Wanted *self, const String12 *rhs);

// The String argument has a non-trivial copy ctor/dtor in the real engine, so the
// C++ ABI passes it by invisible reference -> it arrives as a pointer in r2.
extern "C" Wanted *
Wanted_ctor(Wanted *self, int p1, const String12 &p2, int p3, int p4, bool p5,
            int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14)
{
    Wanted *r = String_default_ctor(self);
    r->f_c = p1;
    String_assign(r, &p2);
    char *b = (char *)self;
    *(int *)(b + 0x10) = p3;
    *(int *)(b + 0x14) = p4;
    *(uint8_t *)(b + 0x18) = p5;
    *(uint16_t *)(b + 0x50) = 0;
    *(int *)(b + 0x44) = -1;
    *(int *)(b + 0x48) = -1;
    *(int *)(b + 0x4c) = -1;
    *(int *)(b + 0x1c) = p6;
    *(int *)(b + 0x20) = p7;
    *(int *)(b + 0x24) = p8;
    *(int *)(b + 0x28) = p9;
    *(int *)(b + 0x2c) = p10;
    *(int *)(b + 0x30) = p11;
    *(int *)(b + 0x34) = p12;
    *(int *)(b + 0x38) = p13;
    *(int *)(b + 0x3c) = p14;
    return self;
}
