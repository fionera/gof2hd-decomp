#include "class.h"

__attribute__((visibility("hidden"))) extern void *g_String_vtable;

// Set(long long): the char ctor forwards a sign/zero-extended value into the 64-bit Set.
extern "C" void String_Set_longlong(String *self, long long v);

// AbyssEngine::String::String(char) - build a string from a single character's numeric value.
extern "C" String *String_ctor_charval(String *self, char c)
{
    F<void *>(self, 0x0) = g_String_vtable;
    F<void *>(self, 0x4) = 0;
    String_Set_longlong(self, (long long)c);
    return self;
}
