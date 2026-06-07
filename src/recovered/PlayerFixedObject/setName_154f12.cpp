#include "class.h"

// Tail-call into AbyssEngine::String::operator= (or move-assign): dst = this+0x1ac, src = r1 (the String arg).
extern "C" void *String_assign(void *dst);

extern "C" void *PlayerFixedObject_setName(PlayerFixedObject *self)
{
    return String_assign((char *)self + 0x1ac);
}
