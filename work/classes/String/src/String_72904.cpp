#include "class.h"

__attribute__((visibility("hidden"))) extern void *g_String_vtable;

extern "C" void String_Set_longlong(String *self, long long v);

// AbyssEngine::String::String(long long) - decimal string of a 64-bit value.
extern "C" String *String_ctor_longlong(String *self, long long v)
{
    F<void *>(self, 0x0) = g_String_vtable;
    F<void *>(self, 0x4) = 0;
    String_Set_longlong(self, v);
    return self;
}
