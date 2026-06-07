#include "class.h"

extern "C" void String_ctor(void *s);   // AbyssEngine::String::String()
extern "C" void Hud_init(Hud *self);

extern "C" Hud *Hud_Hud(Hud *self)
{
    int i = 0x1c;
    do {
        String_ctor((char *)self + i);
        i = i + 0xc;
    } while (i != 0x10c);
    String_ctor((char *)self + 0x1e0);
    String_ctor((char *)self + 500);
    String_ctor((char *)self + 0x200);
    String_ctor((char *)self + 0x228);
    String_ctor((char *)self + 0x3b4);
    String_ctor((char *)self + 0x51c);
    Hud_init(self);
    return self;
}
