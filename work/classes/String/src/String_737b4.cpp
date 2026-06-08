#include "class.h"

__attribute__((visibility("hidden"))) extern void *g_String_vtable;

extern "C" void String_Set_longlong(String *self, long long v);

// AbyssEngine::String::String(int) - decimal string of an int (sign-extended into Set(long long)).
extern "C" String *String_ctor_int(String *self, int v)
{
    F<void *>(self, 0x0) = g_String_vtable;
    F<void *>(self, 0x4) = 0;
    String_Set_longlong(self, (long long)v);
    return self;
}
