#include "class.h"

extern "C" void *String_default_ctor(void *s);         // String::String() -> this
extern "C" void String_assign(void *dst, void *src);   // operator=(String*, String*)

// Agent::Agent(int kind, String name, int p4, int p5, int p6, bool p7,
//              int p8, int p9, int p10, int p11)
extern "C" Agent *Agent_ctor(Agent *self, unsigned kind, void *name,
                             int p4, int p5, int p6, char p7,
                             int p8, int p9, int p10, int p11)
{
    void *s0 = String_default_ctor(self);
    String_default_ctor((char *)s0 + 0x18);
    String_default_ctor((char *)self + 0x6c);
    String_default_ctor((char *)self + 0x78);
    F<unsigned>(self, 0x40) = kind;
    String_assign(self, name);
    F<int>(self, 0x44) = p4;
    F<int>(self, 0x48) = p5;
    F<int>(self, 0x4c) = p6;
    F<char>(self, 0x50) = p7;
    F<int>(self, 0x54) = 0;
    F<int>(self, 0x30) = -1;
    F<int>(self, 0x5c) = -1;
    F<int>(self, 0x64) = p8;
    if (p8 >= 0)
        F<int>(self, 0x5c) = 4;
    F<int>(self, 0x68) = p9;
    if (p9 >= 0)
        F<int>(self, 0x5c) = 3;
    F<uint16_t>(self, 0x84) = 0;
    F<uint16_t>(self, 0x24) = 0;
    F<int>(self, 0xc) = 0;
    F<int>(self, 0x10) = 0;
    F<int>(self, 0x14) = 0;
    F<int>(self, 0x34) = 0;
    F<int>(self, 0x38) = 0;
    F<int>(self, 0x3c) = p11;
    F<int>(self, 0x60) = 0;
    F<int>(self, 0x88) = 0;
    F<int>(self, 0x8c) = 0;
    F<int>(self, 0x90) = 0;
    F<int>(self, 0x28) = -1;
    F<int>(self, 0x2c) = -1;
    F<unsigned>(self, 0x58) = kind >> 31;
    if (p10 >= 0)
        F<int>(self, 0x5c) = 8;
    if (kind == 0x19)
        F<int>(self, 0x5c) = 9;
    else if (kind == 0x1a)
        F<int>(self, 0x5c) = 10;
    F<int>(self, 0x94) = p10;
    return self;
}
